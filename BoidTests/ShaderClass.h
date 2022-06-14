#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderClass
{
public:
	GLuint ID;
	ShaderClass()
		:ID{ glCreateProgram() }
	{ }

	ShaderClass(ShaderClass&& shader) noexcept;

	ShaderClass& operator=(ShaderClass&& shader) noexcept;

	~ShaderClass() { destroy(); }

	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource);

	void activate();
	void setMat4(const char* name, const glm::mat4& mat);
	void setVec3(const char* name, float x, float y, float z);
	void setVec3(const char* name, const glm::vec3& vec);
	void setVec4(const char* name, float x, float y, float z, float w);
	void setVec4(const char* name, const glm::vec4& vec);
	void setFloat(const char* name, float x);
	void setBool(const char* name, bool state);
	void setInt(const char* name, int value);
	void destroy();
private:
	void compileErrors(GLuint shader, const char* type);
};

#endif