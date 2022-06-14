#include "ResourceManager.h"


std::map<std::string, std::unique_ptr<ShaderClass>> ResourceManager::m_shaders{};

std::unique_ptr<ShaderClass> ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    std::unique_ptr<ShaderClass> shader{ new ShaderClass() };
    shader->compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

ShaderClass& ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name)
{
    m_shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return *m_shaders[name];
}
ShaderClass& ResourceManager::getShader(const std::string& name)
{
    return *m_shaders[name];
}

void ResourceManager::clear()
{
    m_shaders.clear();
}