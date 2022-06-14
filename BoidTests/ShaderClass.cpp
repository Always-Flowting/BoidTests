#include "ShaderClass.h"

ShaderClass::ShaderClass(ShaderClass&& shader) noexcept
{
	ID = std::move(shader.ID);
}

ShaderClass& ShaderClass::operator=(ShaderClass&& shader) noexcept
{
	if (&shader == this)
	{
		return *this;
	}

	ID = std::move(shader.ID);

	return *this;
}

void ShaderClass::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	GLuint vertexShader, fragmentShader, geometryShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	if (geometrySource)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySource, nullptr);
		glCompileShader(geometryShader);
		compileErrors(geometryShader, "GEOMETRY");
	}
	
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometrySource)
	{
		glAttachShader(ID, geometryShader);
	}
	glLinkProgram(ID);
	// check for link issues
	compileErrors(ID, "PROGRAM");

	// delete the seperate shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometrySource)
	{
		glDeleteShader(geometryShader);
	}
}

void ShaderClass::activate()
{
	glUseProgram(ID);
}

void ShaderClass::setMat4(const char* name, const glm::mat4& mat)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderClass::setVec3(const char* name, float x, float y, float z)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform3f(loc, x, y, z);
}

void ShaderClass::setVec3(const char* name, const glm::vec3& vec)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

void ShaderClass::setVec4(const char* name, float x, float y, float z, float w)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform4f(loc, x, y, z, w);
}

void ShaderClass::setVec4(const char* name, const glm::vec4& vec)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void ShaderClass::setFloat(const char* name, float x)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform1f(loc, x);
}

void ShaderClass::setBool(const char* name, bool state)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform1i(loc, state);
}

void ShaderClass::setInt(const char* name, int value)
{
	activate();
	GLint loc{ glGetUniformLocation(ID, name) };
	glUniform1i(loc, value);
}

void ShaderClass::destroy()
{
	glDeleteProgram(ID);
}

void ShaderClass::compileErrors(GLuint shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (!hasCompiled)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_ERROR\n" << infoLog
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (!hasCompiled)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM::" << type << "::LINKING_ERROR\n" << infoLog
				<< std::endl;
		}
	}
}