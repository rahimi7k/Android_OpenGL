#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H


#include <GLES3/gl3.h>
#include <string>
#include "../Log.h"
#include "../math/Vector3.h"
#include "Texture.h"


class Material {


private:



public:
	Texture texture;
	// Add other textures as needed
	Vector3 ambientColor;
	Vector3 diffuseColor;
	Vector3 specularColor;
	float shininess;
	float transparency;
	float indexOfRefraction;
	// Add other material properties as needed

	Material() {

	}

	Material(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float shininess, float transparency, float indexOfRefraction) {
		this->ambientColor = ambientColor;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
		this->shininess = shininess;
		this->transparency = transparency;
		this->indexOfRefraction = indexOfRefraction;
	}

	~Material(){

	}




	void setTexture(const char* path) {
		texture.loadTexture(path);
	}





};

#endif //GAME_MATERIAL_H
