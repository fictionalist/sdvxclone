#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

namespace Camera {
    glm::mat4 transformMatrix;

    Shader* baseGameShader;

    glm::vec3 position;
    glm::vec3 target;

    void resetTransform();
}

void Camera::init(Shader* s) {
    position = glm::vec3(0.0f);
    target = glm::vec3(0.0f, 0.0f, 100.0f);
    baseGameShader = s;
    resetTransform();
}

void Camera::setPosition(glm::vec3 p) {
    position = p;
    resetTransform();
}

void Camera::lookAt(glm::vec3 t) {
    target = t;
    resetTransform();
}

void Camera::resetTransform() {
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 matrix = glm::lookAt(position, target, upVector);

    transformMatrix = matrix;
    baseGameShader->use();
    baseGameShader->setMat4("uView", transformMatrix);
}