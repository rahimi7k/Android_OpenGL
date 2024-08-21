#ifndef GAME_SIMOBJECT_H
#define GAME_SIMOBJECT_H


#include "Matrix4x4.h"
#include "Vector3.h"
#include "Shader.h"
#include "Sprite.h"
#include "Sprite3d.h"
#include "Log.h"
#include "transform.h"
#include "Texture.h"

class SimObject {

private:
	Matrix4x4* _worldMatrix = nullptr; // Pointer to Matrix4x4
	Sprite* sprite2D = nullptr;

public:

	Transform transform;


	SimObject(float width, float height, float depth = 0.0f){
		Log::e("Simobject consructor is called");

		_worldMatrix = new Matrix4x4(Matrix4x4::identity());

		sprite2D = new Sprite("", width, height);
		sprite2D->load();


	}


	~SimObject() {
		/*delete _worldMatrix; // Clean up dynamically allocated memory
		_worldMatrix = nullptr; // Optional: Set to nullptr for safety

		delete sprite2D;
		sprite2D = nullptr; // Optional: Set to nullptr for safety*/
	}
/*
	//TODO ASK CHATGPT
	// Copy constructor
	SimObject(const SimObject& other) : _worldMatrix(new Matrix4x4(*other._worldMatrix)), sprite2D(nullptr), transform(other.transform) {
		if (other.sprite2D) {
			sprite2D = new Sprite(*other.sprite2D);
		}
		/if (other.sprite3D) {
			sprite3D = new Sprite3D(*other.sprite3D);
		}/
	}

// Copy assignment operator
	SimObject& operator=(const SimObject& other) {
		if (this == &other) {
			return *this; // Handle self-assignment
		}

		// Clean up existing resources
		delete _worldMatrix;
		delete sprite2D;

		// Allocate new resources and copy the values
		_worldMatrix = new Matrix4x4(*other._worldMatrix);
		sprite2D = other.sprite2D ? new Sprite(*other.sprite2D) : nullptr;
		transform = other.transform;

		return *this;
	}
*/


/** Returns the world position of this entity. */
	Vector3 getWorldPosition() {
		return Vector3(_worldMatrix->data()[12], _worldMatrix->data()[13], _worldMatrix->data()[14]);
	}


	void render(Shader shader) {


		//transform.setPosition(0.2f, 0);
		//transform.setScale(1.5f, 1.5f);


		_worldMatrix->copyFrom(transform.getTransformationMatrix());


		if (sprite2D != nullptr) {
			sprite2D->draw(shader, *_worldMatrix);
		}

	}



	void setImage2D(const char* name, int width, int height, int x = 0, int y = 0, int widthImg= 0, int heightImg= 0) {
		sprite2D->loadTexture(name, width, height, x, y, widthImg, heightImg);
	}

	};

#endif //GAME_SIMOBJECT_H