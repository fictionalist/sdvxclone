#include "Shader.hpp"

#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Logging.hpp"

Shader::Shader() {
    programID = glCreateProgram();
    locationMap = std::unordered_map<std::string, unsigned int>();
    shaderCount = 0;
}

bool Shader::loadShader(std::string path, unsigned int shaderType) {
    if (shaderCount >= 4) {
        Logging::error("Tried to attach shader \"%s\", but shader program has too many shaders already.", path.c_str());
        return false;
    }
    std::ifstream i(path.c_str(), std::ifstream::binary);
    if (!i.good()) {  
        Logging::error("Failed to open shader \"%s\"", path.c_str());      
        return false;
    }

    std::stringstream ss;

    while (i.good()) {
        std::string buf;
        std::getline(i, buf);
        ss << buf << "\n";
    }

    std::string shaderSourceStr = ss.str();
    const char* shaderSource = shaderSourceStr.c_str();

    unsigned int shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, (const char**)&shaderSource, NULL);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        Logging::error("Shader \"%s\" compilation error: %s", path.c_str(), infoLog);
        Logging::error("Shader source:\n%s", shaderSource);
        glDeleteShader(shaderID);
        return false;
    }

    glAttachShader(programID, shaderID);
    attachedShaders[shaderCount++] = shaderID;

    return true;
}

bool Shader::link() {
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, 0, infoLog);
        Logging::error("Shader program %d linking error: %s", programID, infoLog);
    }

    for (unsigned int i = 0; i < shaderCount; i++) {
        glDeleteShader(attachedShaders[i]);
    }
    shaderCount = 0;
    return true;
}

unsigned int Shader::getLocationAddr(std::string location) {
    bool foundLocation = false;
    for (const auto loc : locationMap) {
        if (loc.first.compare(location.c_str()) == 0) {
            foundLocation = true;
            return loc.second;
        }
    }
    unsigned int locationAddr = glGetUniformLocation(programID, location.c_str());
    if (locationAddr == -1) {
        //Logging::error("Shader program %d - no location corresponds to \"%s\"", programID, location.c_str());
        return locationAddr;
    }
    
    locationMap.insert(std::make_pair(location, locationAddr));
    return locationAddr;
}

void Shader::setInt(std::string location, int i) {
    unsigned int locationAddr = getLocationAddr(location);
    if (locationAddr != -1) {
        glUniform1i(locationAddr, i);
    } 
}

void Shader::setMat4(std::string location, glm::mat4 m) {
    unsigned int locationAddr = getLocationAddr(location);
    if (locationAddr != -1) {
        glUniformMatrix4fv(locationAddr, 1, GL_FALSE, glm::value_ptr(m));
    } 
}

void Shader::setVec4(std::string location, glm::vec4 v) {
    unsigned int locationAddr = getLocationAddr(location);
    if (locationAddr != -1) {
        glUniform4fv(locationAddr, 1, glm::value_ptr(v));
    }
}

void Shader::use() {
    glUseProgram(programID);
}