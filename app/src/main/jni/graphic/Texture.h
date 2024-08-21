#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GLES3/gl3.h>
#include <jni.h>


#include <iostream>
#include "../Log.h"
#include "../AssetManager.h"


class Texture {
private:
	std::string _name;
	GLuint _texture;
	bool _isLoaded;
	int _width;
	int _height;
	const unsigned int _desiredChannel = 4;
	const GLint LEVEL = 0;
	const GLint BORDER = 0;


	bool isPowerOf2(int value) const;

public:
	int channel;



	Texture();
	~Texture();




	// Public methods
	void destroy();
	void loadTexture(const char* path);

	void activate(int textureUnit = 0) const;
	void bind() const;
	void unbind() const;

	const std::string& getName() const;
	bool isLoaded() const;
	int getWidth() const;
	int getHeight() const;



};

#endif // TEXTURE_H