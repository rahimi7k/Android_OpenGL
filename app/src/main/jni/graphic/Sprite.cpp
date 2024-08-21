#include "Sprite.h"
#include "../math/Matrix4x4.h"
//#include "MaterialManager.h"

Sprite::Sprite(const std::string& materialName, float width, float height)
        : _materialName(materialName), _width(width), _height(height), _origin(Vector3::zero()),
          _buffer(),
          _modelLocation(-1), _colorLocation(-1), _textureLocation(-1) {


}

Sprite::~Sprite() {
    destroy();
}

void Sprite::load() {

	AttributeInfo positionAttribute;
	positionAttribute.location = 0; // Position attribute location
	positionAttribute.size = 3;
	_buffer.addAttributeLocation(positionAttribute);

	AttributeInfo textCoorAttribute;
	textCoorAttribute.location = 1; // Texture coordinate attribute location
	textCoorAttribute.size = 2;
	_buffer.addAttributeLocation(textCoorAttribute);

	AttributeInfo faceIndexAttribute;
	faceIndexAttribute.location = 2; // Face index attribute location
	faceIndexAttribute.size = 1;
	_buffer.addAttributeLocation(faceIndexAttribute);


	/*AttributeInfo colorAttribute;
	colorAttribute.location = 2; // Color attribute location
	colorAttribute.size = 4;      // RGBA components
	_buffer.addAttributeLocation(colorAttribute);*/

	_origin.x = 0.5f;
	_origin.y = 0.5f;
	float minX = -(_width * _origin.x);
	float maxX = _width * (1.0f - _origin.x);
	float minY = -(_height * _origin.y);
	float maxY = _height * (1.0f - _origin.y);

	/*
	 _vertices = {
			 // x, y, z, u, v
			 // First triangle
			 Vertex(minX, minY, 0, 0, 0),
			 Vertex(minX, maxY, 0, 0, 0.75f),
			 Vertex(maxX, maxY, 0, 0.75f, 0.75f),
			 // Second triangle
			 Vertex(maxX, maxY, 0, 0.75f, 0.75f),
			 Vertex(maxX, minY, 0, 0.75f, 0),
			 Vertex(minX, minY, 0, 0, 0)

			 // x, y, z, u, v, r, g, b
			 //Vertex(minX, minY, 0, 0, 0, 1.0f, 0.0f, 0.0f),    // Bottom-left, red
			 //Vertex(minX, maxY, 0, 0, 1.0f, 0.0f, 1.0f, 0.55f), // Top-left, green
			 //Vertex(maxX, maxY, 0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f), // Top-right, blue
			 //Vertex(maxX, maxY, 0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f), // Top-right, blue
			 //Vertex(maxX, minY, 0, 1.0f, 0, 1.0f, 1.0f, 0.0f),  // Bottom-right, yellow
			 //Vertex(minX, minY, 0, 0, 0, 1.0f, 0.0f, 0.0f)     // Bottom-left, red (repeat for triangle fan)
	 };
	 * */



	_vertices = {
			 // x, y, z, u, v
			// First triangle
			minX, minY, 0, 0, 0, 0, // Bottom-left
			minX, maxY, 0, 0, 1.0f, 0, // Top-left
			maxX, maxY, 0, 1.0f, 1.0f, 0,  // Top-right

			// Second triangle
			maxX, maxY, 0, 1.0f, 1.0f, 0,  // Top-right
			maxX, minY, 0, 1.0f, 0, 0, // Bottom-right
			minX, minY, 0, 0, 0, 0 // Bottom-left

			/*
			 // x, y, z, 		u, v, 			R,	  G,	b,	  a
			 minX, minY, 0, 	0, 0,			0.0f, 0.0f, 1.0f, 1.0f,  // Bottom-left
			 minX, maxY, 0, 	0, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,// Top-left
			 maxX, maxY, 0, 	1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,// Top-right

			 maxX, maxY, 0, 	1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,// Top-right
			 maxX, minY, 0, 	1.0f, 0,		1.0f, 1.0f, 0.0f, 1.0f, // Bottom-right
			 minX, minY, 0, 	0, 0, 			0.0f, 0.0f, 1.0f, 1.0f // Bottom-left
			 */
	 };

	_buffer.clearData();
    _buffer.upload(_vertices);

	//_texture = new Texture(); // Allocate a new texture
}

void Sprite::update(float time) {
    // Update logic here if necessary
}


void Sprite::draw(Shader& shader, const Matrix4x4& worldMatrix) {
    if (_modelLocation == -1) {
        _modelLocation = shader.getUniformLocation("u_model");
		//_colorLocation = shader.getUniformLocation("u_tint");
		_textureLocation = shader.getUniformLocation("u_Textures[0]");
    }


    glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, worldMatrix.toFloat32Array());

	//GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glUniform4fv(_colorLocation, 1, color);


   if (_texture.isLoaded()) {
		//_texture.activateAndBind(0);
        glUniform1i(_textureLocation, 0); // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
   }

    _buffer.bind(GL_FALSE);
	_buffer.draw();
	_buffer.unbind();
}

void Sprite::destroy() {
    _buffer.destroy();
    //MaterialManager::releaseMaterial(_materialName);
   // _material = nullptr;


	//delete _texture; // Avoid memory leaks by deleting the dynamically allocated Texture
	//_texture = nullptr; // Optionally set the pointer to nullptr
}




void Sprite::recalculate() {
    float minX = -(_width * _origin.x);
    float maxX = _width * (1.0f - _origin.x);
    float minY = -(_height * _origin.y);
    float maxY = _height * (1.0f - _origin.y);

	_vertices = {
			// x, y, z, u, v
			// First triangle
			minX, minY, 0, 0, 0,  // Bottom-left
			minX, maxY, 0, 0, 1.0f,  // Top-left
			maxX, maxY, 0, 1.0f, 1.0f,  // Top-right

			// Second triangle
			maxX, maxY, 0, 1.0f, 1.0f,  // Top-right
			maxX, minY, 0, 1.0f, 0,  // Bottom-right
			minX, minY, 0, 0, 0  // Bottom-left
	};

	_buffer.clearData();
    _buffer.upload(_vertices);
}





void Sprite::loadTexture(const char* name, int x, int y, int widthImg, int heightImg) {

	_texture.loadTexture(name); // Load new texture data


	if ( x == 0 && y == 0 && widthImg == 0 && heightImg == 0) {
		return;
	}

	float imgX1 = (float)x / (float)_texture.getWidth();
	float imgY1 = (float)y / (float)_texture.getHeight();
	float imgX2 = (float)(x + widthImg) / (float)_texture.getWidth();
	float imgY2 = (float)(y + heightImg) / (float)_texture.getHeight();

	std::stringstream ss;
	ss << name;
	Log::e("2D Sprite - name:" + ss.str() + " -- imgX1-" + std::to_string(imgX2) + " -  imgY1-" + std::to_string(imgX2) + "-  imgX2-" + std::to_string(imgX2) + "-  imgY2-" + std::to_string(imgY2));

	float minX = -(_width * _origin.x);
	float maxX = _width * (1.0f - _origin.x);
	float minY = -(_height * _origin.y);
	float maxY = _height * (1.0f - _origin.y);

	_vertices = {
			// x, y, z, u, v
			// First triangle
			minX, minY, 0, imgX1, imgY1, 0,  // Bottom-left
			minX, maxY, 0, imgX1, imgY2, 0,  // Top-left
			maxX, maxY, 0, imgX2, imgY2, 0, // Top-right

			// Second triangle
			maxX, maxY, 0, imgX2, imgY2, 0, // Top-right
			maxX, minY, 0, imgX2, imgY1, 0, // Bottom-right
			minX, minY, 0, imgX1, imgY1, 0 	// Bottom-left
	};

	_buffer.clearData();
	_buffer.upload(_vertices);
	_buffer.unbind();
}





float Sprite::getWidth() const {
    return _width;
}

float Sprite::getHeight() const {
    return _height;
}

const Vector3& Sprite::getOrigin() const {
    return _origin;
}

void Sprite::setWidth(float width) {
    _width = width;
    recalculate();
}

void Sprite::setHeight(float height) {
    _height = height;
    recalculate();
}

void Sprite::setOrigin(float x, float y, float z) {
    _origin.x = x;
    _origin.y = y;
    _origin.z = z;
    recalculate();
}

void Sprite::setWidthAndHeight(float width, float height) {
    _width = width;
    _height = height;
    recalculate();
}
