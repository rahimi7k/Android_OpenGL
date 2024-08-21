#include "Sprite3D.h"




Sprite3D::Sprite3D(const std::string& materialName, float width, float height, float depth)
		: _materialName(materialName),
		  _width(width), _height(height), _depth(depth),
		  _origin(Vector3::zero()),
		  _buffer(),
		  _modelLocation(-1), _colorLocation(-1) {
}

Sprite3D::~Sprite3D() {
	destroy();
}







void Sprite3D::load() {
	_buffer = Buffer();

	AttributeInfo positionAttribute;
	positionAttribute.location = 0; // Position attribute location
	positionAttribute.size = 3;
	_buffer.addAttributeLocation(positionAttribute);

	AttributeInfo textCoorAttribute;
	textCoorAttribute.location = 1; // Texture coordinate attribute location
	textCoorAttribute.size = 2;
	_buffer.addAttributeLocation(textCoorAttribute);


	_origin.x = 0.5f;
	_origin.y = 0.5f;
	_origin.z = 0.5f;
	float minX = -(_width * _origin.x);
	float maxX = _width * (1.0f - _origin.x);
	float minY = -(_height * _origin.y);
	float maxY = _height * (1.0f - _origin.y);
	float minZ = -(_depth * _origin.z);
	float maxZ = _depth * (1.0f - _origin.z);

	 _vertices = {
			// Front face
			minX, minY, maxZ, 0, 0,
			maxX, minY, maxZ, 1, 0,
			maxX, maxY, maxZ, 1, 1,
			minX, maxY, maxZ, 0, 1,
			minX, minY, maxZ, 0, 0,
			maxX, maxY, maxZ, 1, 1,

			// Back face
			minX, minY, minZ, 0, 0,
			maxX, minY, minZ, 1, 0,
			maxX, maxY, minZ, 1, 1,
			minX, maxY, minZ, 0, 1,
			minX, minY, minZ, 0, 0,
			maxX, maxY, minZ, 1, 1,

			 // Right face
			maxX, minY, minZ, 0, 0,
			maxX, maxY, minZ, 1, 0,
			maxX, maxY, maxZ, 1, 1,
			maxX, minY, maxZ, 0, 1,
			maxX, minY, minZ, 0, 0,
			maxX, maxY, maxZ, 1, 1,

			// Left face
			minX, minY, minZ, 0, 0,
			minX, maxY, minZ, 1, 0,
			minX, maxY, maxZ, 1, 1,
			minX, minY, maxZ, 0, 1,
			minX, minY, minZ, 0, 0,
			minX, maxY, maxZ, 1, 1,

			// Top face
			minX, maxY, minZ, 0, 0,
			maxX, maxY, minZ, 1, 0,
			maxX, maxY, maxZ, 1, 1,
			minX, maxY, maxZ, 0, 1,
			minX, maxY, minZ, 0, 0,
			maxX, maxY, maxZ, 1, 1

			// Bottom face
			 /*minX, minY, minZ, 0, 0,
			 maxX, minY, minZ, 1, 0,
			 maxX, minY, maxZ, 1, 1,
			 minX, minY, maxZ, 0, 1,
			 minX, minY, minZ, 0, 0,
			 maxX, minY, maxZ, 1, 1 */
	};


	_buffer.clearData();
	_buffer.upload(_vertices);
}

void Sprite3D::update(float time) {
	// Update logic here if necessary
}

void Sprite3D::draw(Shader& shader, const Matrix4x4& worldMatrix) {
	if (_modelLocation == -1) {
		_modelLocation = shader.getUniformLocation("u_model");
		_textureLocation = shader.getUniformLocation("uTexture");
	}

	glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, worldMatrix.toFloat32Array());

		if (_texture.isLoaded()) {
			_texture.activate(0);
			_texture.bind();
			glUniform1i(_textureLocation, 0);// Tell the texture uniform sampler to use this texture in the shader by binding to texture unit i.
		}

	_buffer.bind(GL_FALSE);
	_buffer.draw();
	_buffer.unbind();

	/*GLint uniformValue; // Adjust the size based on the type of the uniform
	glGetUniformiv(shader.program, _textureLocation[i], &uniformValue);
	Log::e("IOResult: faceIndexLocation: " + std::to_string(i) + " - Value: " + std::to_string(uniformValue));*/
}



void Sprite3D::destroy() {
	_buffer.destroy();

	/*for (int i = 0; i < 6; ++i) {
		delete _texture[i]; // Avoid memory leaks by deleting the dynamically allocated Texture
		_texture[i] = nullptr; // Optionally set the pointer to nullptr
	}
*/
}

void Sprite3D::recalculate() {
	/*float minX = -(_width * _origin.x);
	float maxX = _width * (1.0f - _origin.x);
	float minY = -(_height * _origin.y);
	float maxY = _height * (1.0f - _origin.y);
	float minZ = -(_depth * _origin.z);
	float maxZ = _depth * (1.0f - _origin.z);

	_buffer.clearData();
	_buffer.upload(_vertices);*/
}





void Sprite3D::loadTexture(const char* name) {
	_texture.loadTexture(name);
}

void Sprite3D::setAtlasImage(int faceNumber, int x, int y, int widthImg, int heightImg) {

	if ( x == 0 && y == 0 && widthImg == 0 && heightImg == 0) {
		return;
	}


	float imgX1 = (float)x / (float)_texture.getWidth();
	float imgY1 = (float)y / (float)_texture.getHeight();
	float imgX2 = (float)(x + widthImg) / (float)_texture.getWidth();
	float imgY2 = (float)(y + heightImg) / (float)_texture.getHeight();


	Log::e("3D Sprite - faceNumber:" + std::to_string(faceNumber) + " -- imgX1-" + std::to_string(imgX2) + " -  imgY1-" + std::to_string(imgX2) + "-  imgX2-" + std::to_string(imgX2) + "-  imgY2-" + std::to_string(imgY2));


	_vertices[(faceNumber * 30) + (5 * 0) + 3] = imgX1;
	_vertices[(faceNumber * 30) + (5 * 0) + 4] = imgY1;

	_vertices[(faceNumber * 30) + (5 * 1) + 3] = imgX2;
	_vertices[(faceNumber * 30) + (5 * 1) + 4] = imgY1;

	_vertices[(faceNumber * 30) + (5 * 2) + 3] = imgX2;
	_vertices[(faceNumber * 30) + (5 * 2) + 4] = imgY2;

	_vertices[(faceNumber * 30) + (5 * 3) + 3] = imgX1;
	_vertices[(faceNumber * 30) + (5 * 3) + 4] = imgY2;

	_vertices[(faceNumber * 30) + (5 * 4) + 3] = imgX1;
	_vertices[(faceNumber * 30) + (5 * 4) + 4] = imgY1;

	_vertices[(faceNumber * 30) + (5 * 5) + 3] = imgX2;
	_vertices[(faceNumber * 30) + (5 * 5) + 4] = imgY2;


	_buffer.clearData();
	_buffer.upload(_vertices);
	_buffer.unbind();
}




float Sprite3D::getWidth() const {
	return _width;
}

float Sprite3D::getHeight() const {
	return _height;
}

float Sprite3D::getDepth() const {
	return _depth;
}

const Vector3& Sprite3D::getOrigin() const {
	return _origin;
}

void Sprite3D::setWidth(float width) {
	_width = width;
	recalculate();
}

void Sprite3D::setHeight(float height) {
	_height = height;
	recalculate();
}

void Sprite3D::setWidthAndHeight(float width, float height) {
	_width = width;
	_height = height;
	recalculate();
}

void Sprite3D::setDepth(float depth) {
	_depth = depth;
	recalculate();
}

void Sprite3D::setOrigin(float x, float y, float z) {
	_origin.x = x;
	_origin.y = y;
	_origin.z = z;
	recalculate();
}

void Sprite3D::setDimensions(float width, float height, float depth) {
	_width = width;
	_height = height;
	_depth = depth;
	recalculate();
}



