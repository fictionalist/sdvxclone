#include "Renderable.hpp"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Logging.hpp"
#include "Shader.hpp"

unsigned int Renderable::modelCount = 0;

Renderable::Renderable() {
    vertices = std::vector<Vertex>();
    shader = nullptr;
    texture = nullptr;
    VBO = VAO = 0;
}

void Renderable::addVertex(Vertex v) {
    vertices.push_back(v);
}

void Renderable::setShader(Shader* s) {
    shader = s;
}

void Renderable::buildModel() {
    if ((vertices.size() % 3) != 0 || (vertices.size() == 0)) {
        Logging::error("Malformed model.");
        return;
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position)));
    glEnableVertexAttribArray(1);
    
    ID = modelCount++;
}

void Renderable::setTexture(Texture* t) {
    texture = t;
}

void Renderable::draw() {
    if (VBO == 0) {
        return;
    }
    shader->use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (texture != nullptr) {
        texture->bindTexture(shader);
    } else {
        Texture::unbind(shader);
    }
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}