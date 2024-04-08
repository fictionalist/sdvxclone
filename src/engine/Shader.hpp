#pragma once

#include <string>

class Shader {
private:
    unsigned int programID;
    unsigned int attachedShaders[4];
    unsigned int shaderCount;
public:
    Shader();
    bool loadShader(std::string path, unsigned int shaderType);
    bool link();
    void use();
};