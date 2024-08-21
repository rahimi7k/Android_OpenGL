#include "Shader.h"


Shader::Shader() : program(0) {

}



std::string Shader::getVertexSource() const {
    return R"(
		#version 300 es

		layout(location = 0) in vec3 a_position;	// Attribute location for position
		layout(location = 1) in vec2 a_texCoord;	// Attribute location for texture coordinates
		layout(location = 2) in vec3 a_normal;		// Attribute location for normals
		layout(location = 3) in vec4 a_color;		// Attribute location for vertex colors

		uniform mat4 u_projection;  				// Projection matrix
		uniform mat4 u_model;       				// Model matrix

		// Output variables - Varying
		out vec2 v_texCoord;
		out vec3 v_normal;
		out vec3 fragPosition;
		out vec4 VertexColor;


		void main() {

			//gl_Position = u_projection * u_model * vec4(a_position, 1.0);

    		fragPosition = (u_model * vec4(a_position, 1.0)).xyz; // World space position
			gl_Position = u_projection * vec4(fragPosition, 1.0);

			v_texCoord = a_texCoord;
    		v_normal = normalize(mat3(u_model) * a_normal); // Transform normals
			VertexColor = a_color;

		}
    )";
}


std::string Shader::getFragmentSource() const {
    return R"(
       #version 300 es

		precision mediump float;

		// Input variables from the vertex shader
		in vec2 v_texCoord;
		in vec3 v_normal;
		in vec3 fragPosition;
		in vec4 VertexColor;

		// Output color
		out vec4 fragColor;


		// Uniforms for material properties
		//uniform vec4 u_tint;        // Uniform tint color
		uniform sampler2D u_diffuseTexture;
		uniform sampler2D u_specularTexture;
		uniform vec3 u_ambientColor;
		uniform vec3 u_diffuseColor;
		uniform vec3 u_specularColor;
		uniform float u_shininess;

		// Uniforms for lighting
		uniform vec3 u_lightPosition;
		uniform vec3 u_viewPosition;

		// Check if texture is bound by using a default value
		uniform bool u_hasDiffuseTexture;
		uniform bool u_hasSpecularTexture;



		// Function to calculate the lighting
		vec3 calculateLighting(vec3 normal, vec3 lightDir, vec3 viewDir) {
			// Ambient
			vec3 ambient = 0.1 * u_ambientColor;

			// Diffuse
			float diff = max(dot(normal, lightDir), 0.0);
			vec3 diffuse = diff * u_diffuseColor;

			// Specular
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
			vec3 specular = spec * u_specularColor;

			return ambient + diffuse + specular;
		}



		void main() {
			// Calculate the normal and light direction
			vec3 normal = normalize(v_normal);
			//vec3 lightDir = normalize(u_lightPosition - vec3(gl_FragCoord.xy, 0.0));
			//vec3 viewDir = normalize(u_viewPosition - vec3(gl_FragCoord.xy, 0.0));
			vec3 lightDir = normalize(u_lightPosition - fragPosition);
			vec3 viewDir = normalize(u_viewPosition - fragPosition);

			// Calculate the lighting
			vec3 lighting = calculateLighting(normal, lightDir, viewDir);


			// Get texture colors with default values
			vec4 defaultColor = vec4(1.0, 1.0, 1.0, 1.0); // Default white color

			// Get texture colors
			vec4 diffuseTexColor = u_hasDiffuseTexture ? texture(u_diffuseTexture, v_texCoord) : defaultColor;
			vec4 specularTexColor = u_hasSpecularTexture ? texture(u_specularTexture, v_texCoord) : defaultColor;

			//fragColor = diffuseTexColor;
			if (VertexColor.a == 0.0) {
				fragColor = vec4(lighting * diffuseTexColor.rgb, diffuseTexColor.a); //Final color output -- Without vertex color
			} else {
            	fragColor = vec4(lighting * (diffuseTexColor.rgb * VertexColor.rgb), diffuseTexColor.a * VertexColor.a); //Final color output -- With vertex color
			}
			//fragColor = vec4(0.5, 0.5, 1.0, 1.0);
		}

    )";
}



/**
     * The name of the shader
     * */
std::string Shader::getName() {
    return name;
}

/**
 * Use this shader
 * */
void Shader::use() {
    glUseProgram(program);
}

/**
 * Gets the location of an attribute with the provided name
 * @param name The name of the attribute whose location to retrieve
 * */
GLint Shader::getAttributeLocation(const std::string& name) {
    if (attributes.find(name) == attributes.end()) {
        throw std::runtime_error("Unable to find attribute named: " + name + " in shader named: " + name);
    }
    return attributes[name];
}

/**
 * Gets the location of a uniform with the provided name
 * @param name The name of the uniform whose location to retrieve
 * */
GLint Shader::getUniformLocation(const std::string& name) {

    if (uniforms.find(name) == uniforms.end()) {
        throw std::runtime_error("IOResult: Unable to find uniform named: " + name + " in shader named: " + name);
       //Log::e("IOResult: Unable to find uniform named: " + name + " in shader named: " + name);
		return -1;
    }
    return uniforms[name];
}




void Shader::load() {
	std::string vertexCode = getVertexSource();
    std::string fragmentCode = getFragmentSource();
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[1024];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");


    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");


    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		Log::e("OpenGL Error First one: " + std::to_string(error));
	}

    detectAttributes();
    detectUniforms();
}


void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        	Log::e("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
		}
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			Log::e("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
		}
    }
}


void Shader::detectAttributes() {
	Log::e("IOResult - Shader attribute load is called!");

	GLint attributeCount = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
	Log::e("IOResult - GL_ACTIVE_ATTRIBUTES count: " + std::to_string(attributeCount));

	for (GLint i = 0; i < attributeCount; ++i) {
        GLchar name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(program, i, sizeof(name), &length, &size, &type, name);
		GLint location = glGetAttribLocation(program, name);

		std::string str;
		if (location == -1) {
			Log::e("IOResult - Warning: Attribute not found: " +  str.assign(name));
		} else {
			Log::e("IOResult -detectAttributes - name: " + str.assign(name));
		}

		attributes[name] = location;
    }
}

void Shader::detectUniforms() {
    GLint uniformCount = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);

	Log::e("IOResult - GL_ACTIVE_UNIFORMS count: " + std::to_string(uniformCount));

	for (GLint i = 0; i < uniformCount; ++i) {
        GLchar name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);
		if (size > 1) {

			std::string uniformName(name);
			size_t arrayStart = uniformName.find('[');
			if (arrayStart != std::string::npos) {
				uniformName = uniformName.substr(0, arrayStart);
			}
			for (int j = 0; j < size; j++) {
				std::string elementName = std::string(uniformName) + "[" + std::to_string(j) + "]";
				GLint location = glGetUniformLocation(program, elementName.c_str());

				if (location == -1) {
					Log::e("IOResult - Warning: Uniform not found: " +  elementName);
				} else {
					Log::e("IOResult -detectUniforms - name: " + elementName + " - location: " + std::to_string(location));
					uniforms[elementName] = location;
				}
			}

		} else {
			GLint location = glGetUniformLocation(program, name);

			std::string str;
			if (location == -1) {
				Log::e("IOResult - Warning: Uniform not found: " +  str.assign(name));
			} else {
				Log::e("IOResult -detectUniforms - name: " + str.assign(name));
				uniforms[name] = location;
			}
		}


    }
}












