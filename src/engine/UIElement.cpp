#include "UIElement.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.hpp"

static float order = 0.0f;

UIElement::UIElement() {
    position = glm::ivec2(0);
    origin = glm::ivec2(0);
    size = glm::ivec2(0);
    rotation = 0.0f;

    setShader(Renderer::interfaceShader);
    resetTransform();
}

void UIElement::resetTransform() {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(position, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::translate(matrix, glm::vec3(origin, 0.0f));
    transformMatrix = matrix;
}

void UIElement::setRotation(float f) {
    rotation = f;
    resetTransform();
}

void UIElement::setOrigin(glm::ivec2 o) {
    origin = o;
    resetTransform();
}

void UIElement::setSize(glm::ivec2 s) {
    size = s;
    vertices.clear();
    
    vertices.push_back({{(float)size.x,       0.0f, order}, {1.0f, 0.0f}});
    vertices.push_back({{      0.0f,       0.0f, order}, {0.0f, 0.0f}});
    vertices.push_back({{      0.0f, (float)size.y, order}, {0.0f, 1.0f}});
    vertices.push_back({{(float)size.x,       0.0f, order}, {1.0f, 0.0f}});
    vertices.push_back({{      0.0f, (float)size.y, order}, {0.0f, 1.0f}});
    vertices.push_back({{(float)size.x, (float)size.y, order}, {1.0f, 1.0f}});
    order += 0.0001f;
    buildModel();
}

void UIElement::setPosition(glm::ivec2 p) {
    position = p;
    resetTransform();
}

void UIElement::draw() {
    if (VBO == 0) {
        return;
    }
    shader->use();
    shader->setInt("uModelID", ID);
    shader->setMat4("uModel", transformMatrix);
    if (texture != nullptr) {
        texture->bindTexture(shader);
    } else {
        Texture::unbind(shader);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}