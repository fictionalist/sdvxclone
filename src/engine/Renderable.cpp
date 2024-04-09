#include "Renderable.hpp"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Logging.hpp"
#include "Shader.hpp"

Renderable::Renderable() {
    vertices = std::vector<glm::vec3>();
    shader = nullptr;
    VBO = VAO = 0;

    addVertex(glm::vec3(10.0f, 10.0f, 0.0f));
    addVertex(glm::vec3(0.0f, 10.0f, 0.0f));
    addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
    addVertex(glm::vec3(10.0f, 10.0f, 0.0f));
    addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
    addVertex(glm::vec3(10.0f, 0.0f, 0.0f));
    buildModel();
}

void Renderable::addVertex(glm::vec3 v) {
    vertices.push_back(v);
}

void Renderable::setShader(Shader* s) {
    shader = s;
}

void Renderable::buildModel() {
    if ((vertices.size() % 3) != 0) {
        Logging::error("Malformed model.");
        return;
    }
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 3, &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderable::draw() {
    if (VBO == 0) {
        return;
    }
    shader->use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}