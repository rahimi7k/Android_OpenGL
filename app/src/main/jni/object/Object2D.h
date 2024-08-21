#ifndef GAME_OBJECT2D_H
#define GAME_OBJECT2D_H


#include "Object.h"

class Object2D : public Object {

private:

	std::string name;

	GLint* _modelLocation;
	float _width;
	float _height;
	Vector3 _origin;

	Mesh mesh;

	float loopIndex = 0.0f;
	float accelerator = 0.0f;

public:

	Object2D(float width, float height) : mesh("Object2D") {
		_worldMatrix = new Matrix4x4(Matrix4x4::identity());
		_width = width;
		_height = height;
	}

	~Object2D(){

	}


	void loadOBJ(const char* imageName, unsigned int x, unsigned int y, unsigned int width, unsigned int height, Vector3 origin) {
		name = imageName;
		_origin = origin;


		Vector3 ambientColor = Vector3::one(), diffuseColor= Vector3::one(), specularColor= Vector3::one();
		float shininess = 0.0f, transparency = 1.0f, ior = 1.0f; // Initialize defaults

		mesh.setMaterial(ambientColor, diffuseColor, specularColor, shininess, transparency, ior);

		if (imageName != nullptr) {
			Log::e("Diffuse imageName path: " + std::string(imageName));
			mesh.material->setTexture(imageName);
		}



		float imgX1 = (float)x / (float)mesh.material->texture.getWidth();
		float imgY1 = (float)y / (float)mesh.material->texture.getHeight();
		float imgX2 = (float)(x + width) / (float)mesh.material->texture.getWidth();
		float imgY2 = (float)(y + height) / (float)mesh.material->texture.getHeight();

		std::stringstream ss;
		ss << name;
		Log::e("2D Sprite - name:" + ss.str() + " -- imgX1-" + std::to_string(imgX2) + " -  imgY1-" + std::to_string(imgX2) + "-  imgX2-" + std::to_string(imgX2) + "-  imgY2-" + std::to_string(imgY2));

		minXInitial = -(_width * _origin.x);
		maxXInitial = _width * (1.0f - _origin.x);
		minYInitial = -(_height * _origin.y);
		maxYInitial = _height * (1.0f - _origin.y);


		std::vector<float> vertices = {
				// x, y, z,				TexCoord			Normal		Color
				minXInitial, minYInitial, -1.0f,		imgX1, imgY1,		0, 0, 0,	0.5f, 0.5f, 0.5f, 1,
				minXInitial, maxYInitial, -1.0f, 		imgX1, imgY2,		0, 0, 0,	0.5f, 0.5f, 0.5f, 1,
				maxXInitial, maxYInitial, -1.0f, 		imgX2, imgY2,		0, 0, 0,	0.5f, 0.5f, 0.5f, 1,
				maxXInitial, minYInitial, -1.0f, 		imgX2, imgY1,		0, 0, 0,	0.5f, 0.5f, 0.5f, 1
		};

		std::vector<unsigned int> indices = {
				0, 1, 2,
				2, 3, 0
		};
		mesh.init(vertices, indices);


	}




	void draw(Shader& shader) override {
		/*if (_modelLocation == nullptr) {
			*_modelLocation = shader.getUniformLocation("u_model");//TODO -TEST
		}*/
		_worldMatrix->copyFrom(transform.getTransformationMatrix());
		glUniformMatrix4fv(shader.getUniformLocation("u_model"), 1, GL_FALSE, _worldMatrix->toFloat32Array());
		mesh.draw(shader);
	}







	void checkClick(float x, float y) override {
		//Log::e("x: " + std::to_string(x) + " - " + "y: " + std::to_string(y));


		if (minX <= x && x <= maxX && minY <= y && y <= maxY ) {
			Log::e("OnClick " + name);
		}

	}


	void setPosition(float x, float y, float z) override {
		transform.setPosition(x, y, z);
		updatePoints();
	}

	void setRotation(float x, float y, float z) override {
		transform.setRotation(x, y, z);
		updatePoints();
	}

	void setScale(float scale) override {
		transform.setScale(scale);
		updatePoints();
	}


	void updatePoints() override {
Log::e("AssetManager::projectionMatrix " + std::to_string(AssetManager::projectionMatrix.data()[0]) + " -- " + std::to_string(AssetManager::projectionMatrix.data()[5]));
		Matrix4x4 multi = Matrix4x4::multiply(AssetManager::projectionMatrix, transform.getTransformationMatrix());

		minX =  multi.data()[0] * minXInitial;
		maxX =  multi.data()[0] * maxXInitial;
		minY =  multi.data()[5] * minYInitial;
		maxY =  multi.data()[5] * maxYInitial;
		//minZ =  multi.data()[10] * minZInitial;
		//maxZ =  multi.data()[10] * maxZInitial;
	}








};


#endif //GAME_OBJECT2D_H