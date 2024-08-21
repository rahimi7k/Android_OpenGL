#include "Texture.h"



Texture::Texture() : _isLoaded(false) {

	// Generate and bind texture
	glGenTextures(1, &_texture);
	/*bind();
	unsigned char whitePixel[4] = { 255, 255, 255, 255 }; // One pixel white
	// Initially load an empty texture
	glTexImage2D(GL_TEXTURE_2D, LEVEL, GL_RGBA, 1, 1, BORDER, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
	unbind();*/
}

Texture::~Texture() {
	/*if (_isLoaded) {
		glDeleteTextures(1, &_texture);
		_isLoaded = false;
	}*/
}


void Texture::destroy() {
	/*if (_isLoaded) {
		glDeleteTextures(1, &_texture);
		_isLoaded = false;
	}*/
}



void Texture::loadTexture(const char* path) {

    unsigned char* imageData = AssetManager::loadImage(("image/" + std::string(path)).c_str(), _width, _height, channel, _desiredChannel);
	if (imageData == nullptr) {
		Log::e("Failed to load image data with stb_image");
		return;
	}

	bind();

	GLenum format;
	if (_desiredChannel == 1) {
		format = GL_RED;
	} else if (_desiredChannel == 3) {
		format = GL_RGB;
	} else if (_desiredChannel == 4) {
		format = GL_RGBA;
	} else {
		AssetManager::freeImage(imageData);
		Log::e("Unsupported texture format: " + std::string(path));
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, imageData);

	if (isPowerOf2(_width) && isPowerOf2(_height)) {
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		//GL_REPEAT: The default behavior for textures. Repeats the texture image.
		//GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
		//GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		//GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

		// Do not generate a mip map and clamp wrapping to edge
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	AssetManager::freeImage(imageData);
	//unbind();
	_isLoaded = true;
}


bool Texture::isPowerOf2(int value) const {
	return (value & (value - 1)) == 0;
}


void Texture::activate(int textureUnit) const {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}



const std::string& Texture::getName() const {
	return _name;
}

bool Texture::isLoaded() const {
	return _isLoaded;
}

int Texture::getWidth() const {
	return _width;
}

int Texture::getHeight() const {
	return _height;
}