#include "Shader.hpp"

#include <string>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "Logging.hpp"

Shader::Shader() {
    programID = glCreateProgram();
    shaderCount = 0;
}

bool Shader::loadShader(std::string path, unsigned int shaderType) {
    if (shaderCount >= 4) {
        Logging::error("Tried to attach shader \"%s\", but shader program has too many shaders already.", path.c_str());
        return false;
    }
    std::ifstream i = std::ifstream(path.c_str());
    if (!i.good()) {  
        Logging::error("Failed to open shader \"%s\"", path.c_str());      
        return false;
    }

    std::stringstream ss;
    ss << i.rdbuf();
    const char* shaderSource = ss.str().c_str();

    unsigned int shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        Logging::error("Shader \"%s\" compilation error: %s", path.c_str(), infoLog);
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

void Shader::use() {
    glUseProgram(programID);
}