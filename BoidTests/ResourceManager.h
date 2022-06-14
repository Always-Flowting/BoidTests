#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <memory>

#include "ShaderClass.h"

class ResourceManager
{
private:
	static std::map<std::string, std::unique_ptr<ShaderClass>> m_shaders;

	static std::unique_ptr<ShaderClass> loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile);

public:
	ResourceManager()
	{ }

	static ShaderClass& loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name);

	static ShaderClass& getShader(const std::string& name);

	static void clear();
};

#endif