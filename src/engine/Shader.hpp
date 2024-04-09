#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader {
private:
    unsigned int programID;
    unsigned int attachedShaders[4];
    unsigned int shaderCount;

    std::unordered_map<std::string, unsigned int> locationMap;
    unsigned int getLocationAddr(std::string locationName);

public:
    Shader();
    bool loadShader(std::string path, unsigned int shaderType);
    bool link();
    void use();
    
    void setMat4(std::string location, glm::mat4);
};