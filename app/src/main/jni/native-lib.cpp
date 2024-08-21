#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <thread>
#include "gl/Shader.h"
#include "Matrix4x4.h"
#include "Vector3.h"

//#include "object/Tank.h"

#include "Log.h"
#include "AssetManager.h"
#include "test.h"
#include "object/Object3D.h"
#include "object/Object2D.h"


#include <vector>

#include <sstream> //for std::stringstream
#include <string>  //for std::string

#include <assimp/port/AndroidJNI/AndroidJNIIOSystem.h>


// Define missing constant if necessary
#ifndef EGL_OPENGL_ES3_BIT_KHR
#define EGL_OPENGL_ES3_BIT_KHR 0x00000040
#endif

EGLDisplay display;
EGLSurface surface;
EGLContext context;
ANativeWindow* window = nullptr;
Shader* shader = nullptr;
//Tank* tank = nullptr;
Test* test = nullptr;

//std::vector<SimObject> backgrounds;

//Add operator+ to std::string
/*
template<class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc>
operator+(const std::basic_string<_CharT, _Traits, _Alloc>& lhs, _CharT rhs) {
	std::basic_string<_CharT, _Traits, _Alloc> result(lhs);
	result.push_back(rhs);
	return result;
}*/

int width;
int height;
float left;
float right;
float bottom;
float top;
float aspectRatio;

float loopIndex = 0.0f;

float oldX = 0;
float oldY = 0;

	std::vector<Object*> listObject;



void initEGL(ANativeWindow* window) {
    // Get default display

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY) {
		Log::e("Unable to get EGL display");
		return;
	}

	if (!eglInitialize(display, nullptr, nullptr)) {
		Log::e("Unable to initialize EGL");
		return;
	}


	EGLint configAttribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 24,
			EGL_STENCIL_SIZE, 8,
			EGL_NONE
	};

    // Choose EGL config
    EGLConfig config;
	EGLint numConfigs;

	if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs < 1) {
		Log::e("Unable to choose EGL config");
		return;
	}


    // Create EGL context
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		Log::e("Unable to create EGL context");
		return;
	}

    // Create EGL surface
	surface = eglCreateWindowSurface(display, config, window, nullptr);
	if (surface == EGL_NO_SURFACE) {
		Log::e("Unable to create EGL surface");
		return;
	}

	// Make the context current
	if (!eglMakeCurrent(display, surface, surface, context)) {
		Log::e("Unable to make EGL context current");
		return;
	}





	// Clear any pre-existing errors
	while (glGetError() != GL_NO_ERROR) {}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e("OpenGL Error First one: " + std::to_string(error));
	}


	// Enable blending - Transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);


	//test = new Test();
	//test->SSSSSSSS("TEST0");
	/*std::vector<Test> tests;

	for (int p = 0; p < 9; p++) {
		//tests.emplace_back();
		Test* test1 = new Test();
		tests.push_back(*test1);
		const void * address = static_cast<const void*>(&tests[p]);
		std::stringstream ss;
		ss << address;
		std::string name = ss.str();
		tests[p].SSSSSSSS(name);
		test1 = nullptr;
	}

	for (int p = 0; p < 9; p++) {
		tests[p].dddddddddd();
	}*/



	shader = new Shader();

	// Create and load the shader
	shader->load();
	// Use the shader program
	shader->use();
        Log::e("IOResult:Shader Completed!");


	//Zoom
	/*
	float zoom = 16.0f / 14.0f; // 114% zoom in
	float left = -(float)viewPortWidth / 240, right = (float)viewPortWidth / 240, down = -(float)viewPortHeight / 240, up = (float)viewPortHeight / 240;
	viewMatrix = glm::ortho(left / zoom, right / zoom, down / zoom, up / zoom, -1.0f, 1.0f);

	float n = 14.0f * unit_size; // Align 14 units horizontally in the screen.
	const float aspectRatio = (float)viewPortWidth / viewPortHeight;
	float left = -n * 0.5f, right = n * 0.5f, down = -n *
	*/


	width = 810;
	height = 1637;
	//float screenAspectRatio = (float)height / (float)width;
	aspectRatio = (float) width / (float) height; // = 1 / screenAspectRatio;
	left = -1.0f;
	right = 1.0f;
	bottom = -1.0f / aspectRatio;
	top = 1.0f / aspectRatio;


	//Matrix4x4 projectionMatrix = Matrix4x4::orthographic(0.0f, (float) width, (float) height, 0.0f, -100.0f, 1000.0f);
	//Matrix4x4 projectionMatrix = Matrix4x4::orthographic(-aspectRatio, aspectRatio, -1, 1, -100.0f, 1000.0f);
	AssetManager::projectionMatrix = Matrix4x4::orthographic(left, right, bottom, top, -100.0f, 1000.0f);
	glUniformMatrix4fv(shader->getUniformLocation("u_projection"), 1, GL_FALSE, AssetManager::projectionMatrix.toFloat32Array());

	glViewport(0, 0, width, height);



	// Check for OpenGL errors
	error = glGetError();
    if (error != GL_NO_ERROR) {
        Log::e("OpenGL error: " + std::to_string(error));
    }





	Assimp::Importer* importer = new Assimp::Importer;
	Assimp::AndroidJNIIOSystem *ioSystem = new Assimp::AndroidJNIIOSystem("/data/data/com.example.game/filess", AssetManager::assetManager);
	if (ioSystem) {
		importer->SetIOHandler(ioSystem);
	} else {
		Log::e("Failed to create AndroidJNIIOSystem");
		return;
	}



	//Object object;
	Object3D* tree = new Object3D();
	tree->loadOBJ(importer, "image/palm.obj");
	tree->setScale(0.5f);
	tree->setPosition(0.5f, -0.5f, 0.0f);
	listObject.push_back(tree);

	/*
	Object3D* tree1 = new Object3D();
	tree1->loadOBJ(importer, "image/palm.obj");
	tree1->transform.setScale(0.5f);
	tree1->transform.setPosition(0.0f, -0.9f, 0.0f);
	listObject.push_back(tree1);
	*/




	/*Object2D* background = new Object2D(1.0f, 1.0f);
	background->loadOBJ("background.png", 0, 0, 96, 96, Vector3(0.5f, 0.5f, 0));
	listObject.push_back(background);*/

	int imageNumber = 0;
	for (int k = 0; k < 9; k++) {
		Object2D* background = new Object2D(0.5f, 0.5f);
		background->loadOBJ("background.png", imageNumber * 96 , 0, 96, 96, Vector3(0.5f, 0.5f, 0));
		background->setRotation(0.5f, 0, 0);
		switch (k) {
			case 0:
				background->setPosition(0.0f, 0.0f, 0.0f);
				break;
			case 1:
				background->setPosition(0.5f, 0.0f, 0.0f);
				break;
			case 2:
				background->setPosition(0.5f, 0.5f, 0.0f);
				break;
			case 3:
				background->setPosition(0.5f, -0.5f, 0.0f);
				break;
			case 4:
				background->setPosition(0.0f, -0.5f, 0.0f);
				break;
			case 5:
				background->setPosition(-0.5f, -0.5f, 0.0f);
				break;
			case 6:
				background->setPosition(-0.5f, 0.0f, 0.0f);
				break;
			case 7:
				background->setPosition(-0.5f, 0.5f, 0.0f);
				break;
			case 8:
				background->setPosition(0.0f, 0.5f, 0.0f);
				break;
		}
		listObject.push_back(background);


		imageNumber++;
		if (imageNumber > 4) {
			imageNumber = 0;
		}

	}















	/*std::unique_ptr<Object2D> background = std::make_unique<Object2D>();
	background->loadOBJ("background.png");
	listObject.push_back(std::move(background));*/


}


void destroyEGL() {
    /*glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader.getProgram());*/

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);
}


const int ACTION_DOWN = 0;
const int ACTION_UP = 1;
const int ACTION_MOVE = 2;
const int ACTION_CANCEL = 3;
const int ACTION_POINTER_DOWN = 5;
const int ACTION_POINTER_UP = 6;




std::vector<float> getNDCLocation(float x, float y) {

	//normalized device coordinates (NDC) -  Convert the range between -1 to 1
	// (2.0f * x / (float)width): Scales the x-coordinate to the range [0, 2]. - Subtracting 1.0f shifts this range to [-1, 1].
	float ndcX = (2.0f * x / (float)width) - 1.0f;
	float ndcY = 1.0f - (2.0f * y / (float)height);

    //Log::e("x: " + std::to_string(x) + " - " + "y: " + std::to_string(y) + " -- ndcX: " + std::to_string(ndcX) + " - " + "ndcY: " + std::to_string(ndcY));


	// Map NDC to world coordinates - using the orthographic projection bounds:
	// ndcX * (right - left) / 2.0f: Scales the NDC x-coordinate to the range [left, right].
	// Adding (right + left) / 2.0f shifts the center to the midpoint of the left and right bounds.
	float worldX = ndcX * (right - left) / 2.0f + (right + left) / 2.0f;
	float worldY = ndcY * (top - bottom) / 2.0f + (top + bottom) / 2.0f;


    //Log::e("worldX: " + std::to_string(worldX) + " - " + "worldY: " + std::to_string(worldY));


	std::vector<float> a = {worldX , worldY, 0.0f};
	return a;
}


void movement(float x, float y){

	//std::vector<float> oldPointScreen = getNDCLocation(simObject->getWorldPosition().x, simObject->getWorldPosition().y);
	std::vector<float> newPointScreen = getNDCLocation(x, y);


	/*if (oldPointScreen[0] < newPointScreen[0]) {
		simObject->getWorldPosition();
	}*/

	//tank->simObject.transform.setPosition(newPointScreen[0] , newPointScreen[1], 0);
	listObject[0]->setPosition(newPointScreen[0] , newPointScreen[1], 0);
}



extern "C" {

JNIEXPORT void JNICALL
	Java_com_example_game_Native_OnSurfaceCreated(JNIEnv* env, jclass instance, jobject surface) {

		static Log log;
		Log::env = env;
		std::cerr.rdbuf(&log);


		window = ANativeWindow_fromSurface(env, surface);
		initEGL(window);

		// Start a new thread for rendering
		//std::thread renderThread(render);
		// renderThread.detach();
		/*std::thread thread =  std::thread([]() {
			render();
		});
		thread.detach();*/

	}



	JNIEXPORT void JNICALL
	Java_com_example_game_Native_OnSurfaceDestroyed(JNIEnv* env, jclass instance) {
		destroyEGL();
		if (window) {
			ANativeWindow_release(window);
			window = nullptr;
		}
	}


	JNIEXPORT void JNICALL
	Java_com_example_game_Native_Render(JNIEnv* env, jclass instance) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		try {
			//tank->simObject.render(*shader);
			//object.draw(*shader);

			for (int i = 0; i < listObject.size(); i++) {
				listObject[i]->draw(*shader);
			}

			if (loopIndex < 20.0f) {
				loopIndex = loopIndex + 0.007f;

				//object.transform.setRotation(0.5f, loopIndex, 0);
				listObject[0]->setRotation(0.5f, loopIndex, 0);
				/*tree1.transform.setRotation(0.5f, loopIndex, 0);
				tree2.transform.setRotation(0.5f, loopIndex, 0);
				tree3.transform.setRotation(0.5f, loopIndex, 0);
				tree4.transform.setRotation(0.5f, loopIndex, 0);
				tree5.transform.setRotation(0.5f, loopIndex, 0);*/




			} else {
				loopIndex = 0.0f;
			}

		}  catch (const std::exception& e) {
			GLenum error = glGetError();
			//if (error != GL_NO_ERROR) {
			std::string errorMessage = "Caught an exception: ";
			errorMessage += e.what();
			Log::e( "OpenGL Error: " + errorMessage + " (Code: " + std::to_string(error) + ")");
			//}
		}

		eglSwapBuffers(display, surface);
	}



	static float initialDistance = 0.0f;
	static bool isClicked = false;
	static bool isPinching = false;

	JNIEXPORT void JNICALL
	Java_com_example_game_Native_OnTouch1(JNIEnv* env, jclass instance, int action, float x, float y) {

          if (action == ACTION_DOWN) {
                  isClicked = true;
			  std::vector<float> screenPoint = getNDCLocation(x, y);
			  for (auto & object : listObject) {
				  object->checkClick(screenPoint[0], screenPoint[1]);
			  }

		} else if (action == ACTION_MOVE) {
                  if (isClicked) {
                    movement(x, y);
                  }
		} else if (action == ACTION_UP) {

                  if (isClicked) {
                    movement(x, y);
                  }
                  isClicked = false;
                }
	}

	JNIEXPORT void JNICALL
	Java_com_example_game_Native_OnTouch2(JNIEnv* env, jclass instance, int action, float x1, float y1, float x2, float y2) {

		Log::e("Action: " + std::to_string(action));
		if (action == ACTION_POINTER_DOWN) {
			initialDistance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                        isClicked = false;
			isPinching = true;
		} else if (action == ACTION_MOVE) {
			if (isPinching) {
				float currentDistance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				float scale = currentDistance / initialDistance;
                                if (currentDistance > initialDistance) {
									listObject[0]->setScale(listObject[0]->getScale().x + 0.02f);

                                } else if (currentDistance < initialDistance) {
									listObject[0]->setScale(listObject[0]->getScale().x - 0.02f);
                                }

				//Log::e("scale: " + std::to_string(scale));

                                initialDistance = currentDistance;
			}

		} else if (action == ACTION_POINTER_UP) {
				isPinching = false;
		}
	}


	JNIEXPORT void JNICALL
	Java_com_example_game_Native_setAssetManager(JNIEnv* env, jclass instance, jobject manager) {
		AssetManager::setAssetManager(AAssetManager_fromJava(env, manager));
	}


	JNIEXPORT void JNICALL
	Java_com_example_game_Native_Asset(JNIEnv* env, jclass instance, jbyteArray data) {

		jbyte* byteArrayElements = env->GetByteArrayElements(data, nullptr);
		if (byteArrayElements == nullptr) {
			std::cerr << "Failed to get byte array elements" << std::endl;
			return;
		}

		jsize length = env->GetArrayLength(data);
		Log::e("IOResult - byte resid - length: " + std::to_string(length));
		//simObject->setImage("\"image/img_tank.png\"", 197, 549);

		//env->ReleaseByteArrayElements(data, byteArrayElements, JNI_ABORT);

		// Copy the byte array data to the buffer
		//std::vector<jbyte> buffer(length);
		//env->GetByteArrayRegion(data, 0, length, buffer.data());


		/*for (int i =0; i < length; i++) {
			Log::e("IOResult - data[" + std::to_string(i) + "]: " + std::to_string(data[i]));
		}*/

	}

}



