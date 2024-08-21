#ifndef GLBUFFER_H
#define GLBUFFER_H

#include <GLES3/gl3.h>
#include <vector>
#include <stdexcept>
#include <string>
#include "../math/Vertex.h"
#include "../Log.h"



class AttributeInfo {
public:
    /**
     * The location of this attribute
     */
    GLuint location;

    /**
     * The size (number of elements) in this attribute (i.e Vector3 = 3)
     */
    GLint size;

    /**
     * The number of elements from the beginning of the buffer
     */
    GLint offset;

    //AttributeInfo(GLuint loc, GLint sz) : location(loc), size(sz), offset(0) {}
};



class Buffer {
public:
    Buffer(GLenum dataType = GL_FLOAT, GLenum targetBufferType = GL_ARRAY_BUFFER, GLenum mode = GL_TRIANGLES);
    ~Buffer();
    
    void destroy();
/*
	// Copy constructor
	Buffer(const Buffer& other);

	// Copy assignment operator
	Buffer& operator=(const Buffer& other);

	// Move constructor
	Buffer(Buffer&& other) noexcept;

	// Move assignment operator
	Buffer& operator=(Buffer&& other) noexcept;
*/
    void addAttributeLocation(const AttributeInfo& info);
    void upload(const std::vector<float>& vertices);
    void bind(GLboolean normalized = GL_FALSE);
    void unbind();
    void draw();
    void clearData();

private:
    GLboolean hasAttributeLocation;
    GLint elementSize;
    GLint stride;
    GLuint buffer;
    GLenum targetBufferType;
    GLenum dataType;
    GLenum mode;
    GLint typeSize;
    std::vector<float> data;
    std::vector<AttributeInfo> attributes;

    void determineTypeSize();
    void checkGLError(const std::string& errorMessage);
};







#endif // GLBUFFER_H