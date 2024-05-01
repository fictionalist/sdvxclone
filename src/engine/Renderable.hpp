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

    std::vector<Vertex> vertices;

    Shader* shader;
    unsigned int VBO;
    unsigned int VAO;
    bool visible;
    glm::vec4 color;

    Texture* texture;

    void init();

public:
    Renderable();
    virtual void draw();
    void buildModel();
    void addVertex(Vertex);
    void setShader(Shader*);
    void setTexture(Texture*);
    void setColor(glm::vec4);
    void setVisibility(bool);
};