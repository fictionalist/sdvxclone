#include "Object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.hpp"

Object::Object() {
    shader = Renderer::defaultShader;
    model = nullptr;
    texture = nullptr;
    visible = true;
    color = glm::vec4(1.0f);
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    resetTransform();
}

Object::~Object() {
    delete model;
}

void Object::resetTransform() {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transformMatrix = matrix;
}

void Object::setTexture(Texture* t) {
    texture = t;
}

void Object::setPosition(glm::vec3 p) {
    position = p;
    resetTransform();
}

void Object::setRotation(glm::vec3 r) {
    rotation = r;
    resetTransform();
}

glm::vec3 Object::getPosition() {
    return position;
}

void Object::setColor(glm::vec4 c) {
    color = c;
}

void Object::setVisibility(bool b) {
    visible = b;
}

void Object::update(unsigned int deltaTick) {

}

void Object::draw() {
    if (!visible) return;
    if (model != nullptr && shader != nullptr) {
        shader->use();
        shader->setMat4("uModel", transformMatrix);
        shader->setVec4("uColor", color);
        model->draw(shader, texture);
    }
}