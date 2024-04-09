#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"

class Renderable {
private:
    //Renderable() = delete;

    std::vector<glm::vec3> vertices;

    Shader* shader;
    unsigned int VBO;
    unsigned int VAO;

public:
    Renderable();
    virtual void draw();
    void buildModel();
    void addVertex(glm::vec3);
    void setShader(Shader*);
};