#ifndef GAME_OBJECT3D_H
#define GAME_OBJECT3D_H



#include "Object.h"


class Object3D : public Object {

private:

	std::vector<Mesh> meshes;

	float loopIndex = 0.0f;
	float accelerator = 0.0f;

public:
	//Transform transformHelices;


	void processNode(aiNode* node, const aiScene* scene) {
		// Process each mesh located at the current node

		Log::e("mNumMeshes " + std::to_string(node->mNumMeshes));

		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}

		Log::e("mNumChildren " + std::to_string(node->mNumChildren));
		// Then do the same for each child node
		for (unsigned int j = 0; j < node->mNumChildren; j++) {
			processNode(node->mChildren[j], scene);
		}
	}


	void processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<float> vertices;
		std::vector<float> texcoords;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
		std::vector<float> colors;


		vertices.reserve(mesh->mNumVertices * 3);
		texcoords.reserve(mesh->mNumVertices * 2);
		normals.reserve(mesh->mNumVertices * 3);
		colors.reserve(mesh->mNumVertices * 4);


		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			// Positions
			aiVector3D position = mesh->mVertices[i];
			vertices.push_back(position.x);
			vertices.push_back(position.y);
			vertices.push_back(position.z);
			Log::log("Vertex[" + std::to_string(i) + "]: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z));

			// Texture coordinates
			if (mesh->mTextureCoords[0] != nullptr) {
				aiVector3D texcoord = mesh->mTextureCoords[0][i];
				texcoords.push_back(texcoord.x);
				texcoords.push_back(texcoord.y);
				Log::log("Texcoord[" + std::to_string(i) + "]: " + std::to_string(texcoord.x) + ", " + std::to_string(texcoord.y));
			} else {
				texcoords.push_back(0.0f);
				texcoords.push_back(0.0f);
				Log::log("Texcoord[" + std::to_string(i) + "]: 0.0, 0.0");
			}

			// Normals
			aiVector3D normal = mesh->mNormals[i];
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);


			if (mesh->mColors[0] != nullptr) {  // Check if colors are available
				aiColor4D color = mesh->mColors[0][i];
				colors.push_back(color.r);
				colors.push_back(color.g);
				colors.push_back(color.b);
				colors.push_back(color.a);
				Log::log("Color: " + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ", " + std::to_string(color.a));
			} else {
				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(1.0f);
				Log::log("Vertex had no color");
			}
		}


		//Faces in OBJ files can be triangles, quads, or polygons with more than four vertices.
		//Assimp processes all faces as triangles when using aiProcess_Triangulate, so this should be fine.
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			if (face.mNumIndices != 3) {
				Log::e("Warning: Non-triangular face encountered.");
			}
			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j]);
			}
			//Log::e("Loaded mesh with " + std::to_string(vertices.size() / 3) + " vertices and " + std::to_string(indices.size() / 3) + " faces.");
		}


		std::vector<float> arrayBuffer;
		size_t vertexCount = vertices.size() / 3;
		size_t texcoordCount = texcoords.size() / 2;
		size_t normalCount = normals.size() / 3;
		size_t colorCount = colors.size() / 4;

		if (vertexCount == normalCount && vertexCount == texcoordCount && vertexCount == colorCount) {
			arrayBuffer.reserve(vertices.size() + texcoords.size() + normals.size() + colors.size());
			for (size_t i = 0; i < vertexCount; ++i) {
				arrayBuffer.insert(arrayBuffer.end(), { vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2] });
				arrayBuffer.insert(arrayBuffer.end(), { texcoords[2 * i], texcoords[2 * i + 1] });
				arrayBuffer.insert(arrayBuffer.end(), { normals[3 * i], normals[3 * i + 1], normals[3 * i + 2] });
				arrayBuffer.insert(arrayBuffer.end(), { colors[4 * i], colors[4 * i + 1], colors[4 * i + 2], colors[4 * i + 3] });
			}
		}

		meshes.emplace_back(mesh->mName.C_Str());
		//Mesh mesh1(mesh->mName.C_Str(), vertices, texcoords, normals, indices, colors);
		//meshes.push_back(mesh1);
		meshes[meshes.size() - 1].init(arrayBuffer, indices);


		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			aiColor3D ambientColor, diffuseColor, specularColor;
			float shininess = 0.0f, transparency = 1.0f, ior = 1.0f; // Initialize defaults

			mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			mat->Get(AI_MATKEY_SHININESS, shininess);
			mat->Get(AI_MATKEY_OPACITY, transparency);
			mat->Get(AI_MATKEY_REFRACTI, ior);


			meshes[meshes.size() - 1].setMaterial(Vector3(ambientColor.r, ambientColor.g, ambientColor.b),
							  Vector3(diffuseColor.r, diffuseColor.g, diffuseColor.b),
							  Vector3(specularColor.r, specularColor.g, specularColor.b),
							  shininess, transparency, ior);

			// Load diffuse texture
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				const char* texturePath = path.C_Str();
				Log::e("Diffuse texture path: " + std::string(texturePath));
				meshes[meshes.size() - 1].material->setTexture(texturePath);
			}

                        // Load specular texture (if applicable)
			if (mat->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
				const char* texturePath = path.C_Str();
				Log::e("Specular texture path: " + std::string(texturePath));
				//material.specularTextureID = loadTexture(texturePath);
			}
		}


	}



	void loadOBJ(Assimp::Importer* importer, const char* image) {

		_worldMatrix = new Matrix4x4(Matrix4x4::identity());
		//_helicesMatrix = new Matrix4x4(Matrix4x4::identity());


		const aiScene* scene = importer->ReadFile(image, aiProcess_Triangulate);
		//const aiScene* scene = importer->ReadFile(image, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials);
		/*const aiScene* scene = importer->ReadFile(image,
												  aiProcess_Triangulate |
												  aiProcess_JoinIdenticalVertices |
												  aiProcess_SortByPType |
												  aiProcess_ImproveCacheLocality |
												  aiProcess_RemoveRedundantMaterials |
												  aiProcess_FindDegenerates |
												  aiProcess_FindInvalidData |
												  aiProcess_ValidateDataStructure |
												  aiProcess_OptimizeMeshes |
												  aiProcess_GenSmoothNormals |
												  aiProcess_FixInfacingNormals |
												  aiProcess_OptimizeGraph
		);*/
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ERROR::ASSIMP:: " << importer->GetErrorString() << std::endl;
			return;
		}
		processNode(scene->mRootNode, scene);

	}




	void checkClick(float x, float y) override {


	}



	void draw(Shader& shader) override {
		GLint modelLocation = shader.getUniformLocation("u_model");


                /*
		if (accelerator < 20.0f) {
			accelerator = accelerator + 0.05f;
		} else if (accelerator < 80.0f) {
			accelerator = accelerator + 0.3f;
		} else if (accelerator < 200.0f) {
			accelerator = accelerator + 0.7f;
		} else {
			accelerator = 0.0f;
		}
		transformHelices.setRotation(accelerator, 0.0f, 0.0f);
                */


		_worldMatrix->copyFrom(transform.getTransformationMatrix());

		// Iterate over shapes and apply materials
		for (auto& mesh : meshes) {

			/*if (mesh.name == "HELICES") {
				*_worldMatrix = Matrix4x4::multiply(transformHelices.getTransformationMatrix(), *_worldMatrix);
			} else {
				_worldMatrix->copyFrom(transform.getTransformationMatrix());
			}*/

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, _worldMatrix->toFloat32Array());
			mesh.draw(shader);
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
		minX =  transform.getTransformationMatrix().data()[0] * minXInitial;
		maxX =  transform.getTransformationMatrix().data()[0] * maxXInitial;
		minY =  transform.getTransformationMatrix().data()[5] * minYInitial;
		maxY =  transform.getTransformationMatrix().data()[5] * maxYInitial;
		//minZ =  transform.getTransformationMatrix().data()[10] * minZInitial;
		//maxZ =  transform.getTransformationMatrix().data()[10] * maxZInitial;
	}





};


#endif //GAME_OBJECT3D_H