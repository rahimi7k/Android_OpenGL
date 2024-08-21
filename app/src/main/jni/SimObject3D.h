#ifndef GAME_SIMOBJECT3D_H
#define GAME_SIMOBJECT3D_H


#include "Matrix4x4.h"
#include "Vector3.h"
#include "Shader.h"
#include "Sprite3d.h"
#include "Log.h"
#include "transform.h"
#include "Texture.h"

class SimObject3D {

private:
	Matrix4x4* _worldMatrix = nullptr; // Pointer to Matrix4x4

public:

	Transform transform;
	Sprite3D sprite3D;

	SimObject3D(float width, float height, float depth = 0.0f):sprite3D("", width, height, depth){

		_worldMatrix = new Matrix4x4(Matrix4x4::identity());


		//sprite = new Sprite("", width, height);
		//sprite->load();

			sprite3D.load();
	}


	~SimObject3D() {
		delete _worldMatrix; // Clean up dynamically allocated memory
		_worldMatrix = nullptr; // Optional: Set to nullptr for safety

	}
/*
	//TODO ASK CHATGPT
	// Copy constructor
	SimObject3D(const SimObject3D& other) : _worldMatrix(new Matrix4x4(*other._worldMatrix)), sprite3D(nullptr), transform(other.transform) {
			sprite3D = new Sprite3D(*other.sprite3D);
	}

// Copy assignment operator
	SimObject3D& operator=(const SimObject3D& other) {
		if (this == &other) {
			return *this; // Handle self-assignment
		}

		// Clean up existing resources
		delete _worldMatrix;
		delete sprite3D;

		// Allocate new resources and copy the values
		_worldMatrix = new Matrix4x4(*other._worldMatrix);
		sprite3D = other.sprite3D ? new Sprite3D(*other.sprite3D) : nullptr;
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

		sprite3D.draw(shader, *_worldMatrix);

	}


};

#endif //GAME_SIMOBJECT3D_H