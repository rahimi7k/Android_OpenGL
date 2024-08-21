#ifndef GAME_MESH_H
#define GAME_MESH_H

#include <iostream>
#include <vector>
#include <string>
#include <GLES3/gl3.h>

#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Matrix4x4.h"

#include "../gl/Shader.h"
#include "../Log.h"
#include "Material.h"




class Mesh {
private:
	GLuint VAO, VBO, EBO;
	std::vector<unsigned int> indices;
	std::string name;
	bool checkBool = false;

public:
	Material* material;


	Mesh(const std::string& name) : name(name)  {

	}



	void init(const std::vector<float>& arrayBuffer, const std::vector<unsigned int>& indices) {
		this->indices = indices;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, arrayBuffer.size() * sizeof(float), &arrayBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 2 + 3 + 4) * sizeof(float), (void*)0);

		// Vertex Texture Coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (3 + 2 + 3 + 4) * sizeof(float), (void*)(3 * sizeof(float)));

		// Vertex Normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (3 + 2 + 3 + 4) * sizeof(float), (void*)((3 + 2) * sizeof(float)));

		// Vertex Color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, (3 + 2 + 3 + 4) * sizeof(float), (void *) ((3 + 2 + 3) * sizeof(float)));

		glBindVertexArray(0);
	}



	void setUniforms(Shader& shader) {
		if (material == nullptr) {
			return;
		}
		Material material = *this->material;
		GLint hasDiffuseTextureLocation =  shader.getUniformLocation("u_hasDiffuseTexture");
		GLint hasSpecularTextureLocation =  shader.getUniformLocation("u_hasSpecularTexture");

		glUniform1i(hasDiffuseTextureLocation, material.texture.isLoaded());
		glUniform1i(hasSpecularTextureLocation, false);

		if (material.texture.isLoaded()) {
			material.texture.activate(0);
			material.texture.bind();
			GLint textureLocation = shader.getUniformLocation("u_diffuseTexture");
			glUniform1i(textureLocation, 0);
		}

		if (false) {
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, material.specularTextureID);
			//GLint specularTexLocation =  shader.getUniformLocation("u_specularTexture");
			//glUniform1i(specularTexLocation, 1);
		}

		if (!checkBool) {
			checkBool = true;
			Log::e("u_ambientColor " + std::to_string(material.ambientColor.x) + ", " + std::to_string(material.ambientColor.y) + ", " + std::to_string(material.ambientColor.z));
			Log::e("u_diffuseColor " + std::to_string(material.diffuseColor.x) + ", " + std::to_string(material.diffuseColor.y) + ", " + std::to_string(material.diffuseColor.z));
			Log::e("u_specularColor " + std::to_string(material.specularColor.x) + ", " + std::to_string(material.specularColor.y) + ", " + std::to_string(material.specularColor.z));
			Log::e("u_shininess " + std::to_string(material.shininess));
		}

		// Set other material uniforms as needed
		glUniform3f(shader.getUniformLocation("u_ambientColor"), material.ambientColor.x, material.ambientColor.y, material.ambientColor.z);
		glUniform3f(shader.getUniformLocation("u_diffuseColor"), material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
		glUniform3f(shader.getUniformLocation("u_specularColor"), material.specularColor.x, material.specularColor.y, material.specularColor.z);

		glUniform1f(shader.getUniformLocation("u_shininess"), material.shininess);
		glUniform3f(shader.getUniformLocation("u_lightPosition"), 10.0f, 10.0f, 10.0f);
		glUniform3f(shader.getUniformLocation("u_viewPosition"), 0.0f, 0.0f, 5.0f);
	}



	void draw(Shader& shader) {
		setUniforms(shader);

		/*if (material->texture.channel == 4) {
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
		} else {
			glDisable(GL_BLEND);
			glEnable(GL_BLEND);
		}*/

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Restore depth mask and blending state
		//glDepthMask(GL_TRUE);
		//glDisable(GL_BLEND);
	}




	void setMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float shininess, float transparency, float ior) {

		material = new Material(ambientColor, diffuseColor, specularColor, shininess, transparency, ior);
		/*material.ambientColor = ambientColor;
		material.diffuseColor = diffuseColor;
		material.specularColor = specularColor;
		material.shininess = shininess;
		material.transparency = transparency;
		material.indexOfRefraction = ior;*/


		/*Log::e("Ambient Color: " + std::to_string(material.ambientColor.x) + ", " + std::to_string(material.ambientColor.y) + ", " + std::to_string(material.ambientColor.z));
		Log::e("Diffuse Color: " + std::to_string(material.diffuseColor.x) + ", " + std::to_string(material.diffuseColor.y) + ", " + std::to_string(material.diffuseColor.z));
		Log::e("Specular Color: " + std::to_string(material.specularColor.x) + ", " + std::to_string(material.specularColor.y) + ", " + std::to_string(material.specularColor.z));
		Log::e("Shininess: " + std::to_string(material.shininess));
		Log::e("Transparency: " + std::to_string(material.transparency));
		Log::e("Index of Refraction: " + std::to_string(material.indexOfRefraction));*/

	}
};




















#endif //GAME_MESH_H
