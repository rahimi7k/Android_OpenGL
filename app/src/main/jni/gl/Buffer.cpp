#include "Buffer.h"
#include <iostream>

Buffer::Buffer(GLenum dataType, GLenum targetBufferType, GLenum mode)
    : hasAttributeLocation(GL_FALSE), elementSize(0), stride(0), buffer(0),
      targetBufferType(targetBufferType), dataType(dataType), mode(mode), typeSize(0) {

    //this->targetBufferType = targetBufferType;

    switch (dataType) {
        case GL_FLOAT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            typeSize = 4; // 32-bit
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            typeSize = 2; // 16-bit
            break;
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            typeSize = 1; // 8-bit
            break;
        default:
            throw std::runtime_error("Unrecognized data type: " + std::to_string(dataType));
    }

    glGenBuffers(1, &buffer);
	if (buffer == 0) {
		Log::e("IOResult - Failed to create buffer");
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			Log::e("OpenGL Error: " + std::to_string(error));
		}
		return;
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e( "OpenGL Error:  (Code: " + std::to_string(error) + ")");
	}

}

Buffer::~Buffer() {
    destroy();
}

void Buffer::destroy() {
    glDeleteBuffers(1, &buffer);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e( "OpenGL Error:  (Code: " + std::to_string(error) + ")");
	}
}



/*
// Copy constructor
Buffer::Buffer(const Buffer& other)
		: hasAttributeLocation(other.hasAttributeLocation), elementSize(other.elementSize),
		  stride(other.stride), targetBufferType(other.targetBufferType), dataType(other.dataType),
		  mode(other.mode), typeSize(other.typeSize), data(other.data), attributes(other.attributes) {

	glGenBuffers(1, &buffer);
	if (buffer == 0) {
		Log::e("IOResult - Failed to create buffer in copy constructor");
		return;
	}

	glBindBuffer(targetBufferType, other.buffer);
	glBufferData(targetBufferType, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e("OpenGL Error in copy constructor: " + std::to_string(error));
	}

	glBindBuffer(targetBufferType, 0);
}

// Copy assignment operator
Buffer& Buffer::operator=(const Buffer& other) {
	if (this == &other) return *this;

	destroy();

	hasAttributeLocation = other.hasAttributeLocation;
	elementSize = other.elementSize;
	stride = other.stride;
	targetBufferType = other.targetBufferType;
	dataType = other.dataType;
	mode = other.mode;
	typeSize = other.typeSize;
	data = other.data;
	attributes = other.attributes;

	glGenBuffers(1, &buffer);
	if (buffer == 0) {
		Log::e("IOResult - Failed to create buffer in copy assignment operator");
		return *this;
	}

	glBindBuffer(targetBufferType, other.buffer);
	glBufferData(targetBufferType, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e("OpenGL Error in copy assignment operator: " + std::to_string(error));
	}

	glBindBuffer(targetBufferType, 0);

	return *this;
}

// Move constructor
Buffer::Buffer(Buffer&& other) noexcept
		: hasAttributeLocation(other.hasAttributeLocation), elementSize(other.elementSize),
		  stride(other.stride), buffer(other.buffer), targetBufferType(other.targetBufferType),
		  dataType(other.dataType), mode(other.mode), typeSize(other.typeSize),
		  data(std::move(other.data)), attributes(std::move(other.attributes)) {

	other.buffer = 0;
}

// Move assignment operator
Buffer& Buffer::operator=(Buffer&& other) noexcept {
	if (this == &other) return *this;

	destroy();

	hasAttributeLocation = other.hasAttributeLocation;
	elementSize = other.elementSize;
	stride = other.stride;
	buffer = other.buffer;
	targetBufferType = other.targetBufferType;
	dataType = other.dataType;
	mode = other.mode;
	typeSize = other.typeSize;
	data = std::move(other.data);
	attributes = std::move(other.attributes);

	other.buffer = 0;

	return *this;
}
*/





















void Buffer::addAttributeLocation(const AttributeInfo& info) {
    hasAttributeLocation = GL_TRUE;

    AttributeInfo attribute = info;
    attribute.offset = elementSize;
    attributes.push_back(attribute);
    elementSize += attribute.size;
    stride = elementSize * typeSize;
}

void Buffer::upload(const std::vector<float>& vertices) {
    glBindBuffer(targetBufferType, buffer);
	Log::e("Buffer upload is Done");

    //data.clear();

	data = vertices;

   /* for (const Vertex& vertex : vertices) {
		std::vector<float> vertexData = vertex.toArray();
		//data.insert(data.end(), vertexData.begin(), vertexData.end());

		// Push position data
		data.insert(data.end(), vertex.position.toArray().begin(), vertex.position.toArray().end());

		// Push texture coordinates
		data.insert(data.end(), vertex.texCoords.toArray().begin(), vertex.texCoords.toArray().end());

		// Push color data (RGBA)
		//data.insert(data.end(), vertex.color.toArray().begin(), vertex.color.toArray().end());
	}*/

	/*for (u_int i = 0; i < data.size() / elementSize; i++) {
		Log::e("IOResult - Data[" + std::to_string(i) + "]: " +
					   std::to_string(	data[i * 9]) + "  " +
					   std::to_string(	data[(i * 9) + 1]) + "  " +
					   std::to_string(	data[(i * 9) + 2]) + "  " +
					   std::to_string(	data[(i * 9) + 3]) + "  " +
					   std::to_string(	data[(i * 9) + 4]) + "  "+
					   std::to_string(	data[(i * 9) + 5]) + "  "+
					   std::to_string(	data[(i * 9) + 6]) + "  "+
					   std::to_string(	data[(i * 9) + 7]) + "  "+
					   std::to_string(	data[(i * 9) + 8]) + "  ");

	}*/

    //TODO -- DIFFERENT TYPE
    glBufferData(targetBufferType, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e("OpenGL Error First one: " + std::to_string(error));
	}
	unbind();
}



void Buffer::bind(GLboolean normalized) {
    glBindBuffer(targetBufferType, buffer);

    if (hasAttributeLocation) {

		for (const AttributeInfo& attribute : attributes) {
			/*Log::e(
					"location: " + std::to_string(attribute.location) + " "+
					"size: " + std::to_string(attribute.size) +" "+
					"dataType: " + std::to_string(dataType) +" "+
					"elementSize: " + std::to_string(elementSize) +" "+
					"typeSize: " + std::to_string(typeSize) +" "+
					"stride: " + std::to_string(stride) +" "+
					"offset: " + std::to_string(attribute.offset) +" "+
					"offset: " + std::to_string(attribute.offset * typeSize)
					);
				*/

           glVertexAttribPointer(attribute.location, attribute.size, dataType, normalized, stride, (GLvoid*)(attribute.offset * typeSize));
		   glEnableVertexAttribArray(attribute.location);
		}
    }
}

void Buffer::unbind() {
    for (const AttributeInfo& attribute : attributes) {
        glDisableVertexAttribArray(attribute.location);
    }
    glBindBuffer(targetBufferType, 0);
}



void Buffer::draw() {
    if (targetBufferType == GL_ARRAY_BUFFER) {
		/*og::e("IOResult - Draw - data.size() : " + std::to_string(data.size() ));
		Log::e("IOResult - Draw - mode: " + std::to_string(mode ));
		Log::e("IOResult - Draw - elementSize: " + std::to_string(elementSize ));*/

		glDrawArrays(mode, 0, data.size() / elementSize); // data.size() / elementSize

	} else if (targetBufferType == GL_ELEMENT_ARRAY_BUFFER) {
        glDrawElements(mode, data.size(), dataType, 0);
    }
}

void Buffer::clearData() {
    data.clear();
}
