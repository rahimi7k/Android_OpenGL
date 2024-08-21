#ifndef GAME_SPRITE3D_H
#define GAME_SPRITE3D_H

#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include "../gl/Buffer.h"
#include "../gl/Shader.h"
#include "../math/Vector3.h"
#include "../math/Vertex.h"
#include "../math/Matrix4x4.h"
#include "../Log.h"
#include "Texture.h"


class Sprite3D {
public:
	Sprite3D(const std::string& materialName, float width = 100.0f, float height = 100.0f, float depth = 100.0f);
	~Sprite3D();

	void load();
	void update(float time);
	void draw(Shader& shader, const Matrix4x4& worldMatrix);
	void destroy();

	float getWidth() const;
	float getHeight() const;
	float getDepth() const;
	const Vector3& getOrigin() const;

	void setWidth(float width);
	void setDepth(float depth);
	void setHeight(float height);
	void setOrigin(float x, float y, float z);
	void setWidthAndHeight(float width, float height);
	void setDimensions(float width, float height, float depth);



	void loadTexture(const char* name);
	void setAtlasImage(int faceNumber, int x, int y, int widthImg, int heightImg);


private:
	void recalculate();

	std::string _materialName;
	float _width;
	float _height;
	float _depth;
	float _aspectRatio;


	Vector3 _origin;
	Buffer _buffer;
	//Material* _material;
	std::vector<float> _vertices;

	Texture _texture;
	GLint _textureLocation;
	GLint _modelLocation;
	GLint _colorLocation;

};




#endif //GAME_SPRITE3D_H
