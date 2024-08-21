#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <jni.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <GLES3/gl3.h>

#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../AssetManager.h"
#include "../graphic/Mesh.h"
#include "../graphic/Material.h"

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags
#include "assimp/port/AndroidJNI/AndroidJNIIOSystem.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"
#include "../math/transform.h"



class Object {

protected:
	Transform transform;
	Matrix4x4 *_worldMatrix = nullptr; // Pointer to Matrix4x4
	//Matrix4x4* _helicesMatrix = nullptr; // Pointer to Matrix4x4

	virtual void updatePoints() = 0; // Pure virtual function


public:

	float minXInitial;
	float maxXInitial;
	float minYInitial;
	float maxYInitial;

	float minX;
	float maxX;
	float minY;
	float maxY;




	Object() {

	}

	~Object() {
		if (_worldMatrix != nullptr) {
			delete _worldMatrix;
			_worldMatrix = nullptr;
		}
	}

	virtual void setPosition(float x, float y, float z) = 0; // Pure virtual function
	virtual void setRotation(float x, float y, float z) = 0; // Pure virtual function
	virtual void setScale(float scale) = 0; // Pure virtual function

	virtual Vector3 getScale() {
		return transform.getScale();
	}

	virtual void checkClick(float x, float y) = 0; // Pure virtual function

	virtual void draw(Shader& shader) = 0; // Pure virtual function





};

#endif //GAME_OBJECT_H
