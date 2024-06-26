#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader {
private:
    std::string shaderName;
    unsigned int programID;
    unsigned int attachedShaders[4];
    unsigned int shaderCount;

    std::unordered_map<std::string, unsigned int> locationMap;
    unsigned int getLocationAddr(std::string locationName);

public:
    Shader(std::string name);
    bool loadShader(std::string path, unsigned int shaderType);
    bool link();
    void use();
    
    void setInt(std::string location, int);
    void setVec4(std::string location, glm::vec4);
    void setMat4(std::string location, glm::mat4);
};