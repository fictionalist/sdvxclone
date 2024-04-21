#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec2 UV;
};

class Renderable {
protected:
    static unsigned int modelCount;
    unsigned int ID;
    //Renderable() = delete;

    std::vector<Vertex> vertices;

    Shader* shader;
    unsigned int VBO;
    unsigned int VAO;

    Texture* texture;

public:
    Renderable();
    virtual void draw();
    void buildModel();
    void addVertex(Vertex);
    void setShader(Shader*);
    void setTexture(Texture*);
};