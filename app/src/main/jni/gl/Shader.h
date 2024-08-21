#ifndef SHADER_H
#define SHADER_H

#include <GLES3/gl3.h>
#include <string>
//#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Log.h"

class Shader {
public:
    Shader();
    void load();
    GLuint getProgram() const { return program; }
    void use() const { glUseProgram(program); }

    GLint getAttributeLocation(const std::string& name);
    GLint getUniformLocation(const std::string& name);

    std::string getName();
    void use();

	GLuint program;

private:
    std::string name;

	std::string getVertexSource() const;
	std::string getFragmentSource() const;

	void checkCompileErrors(GLuint shader, std::string type);
	void detectAttributes();
	void detectUniforms();
	std::unordered_map<std::string, GLint> attributes;
	std::unordered_map<std::string, GLint> uniforms;


};

#endif // SHADER_H