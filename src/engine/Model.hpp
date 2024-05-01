#pragma once

#include <glm/glm.hpp>

#include "Renderable.hpp"

class Model : public Renderable {
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::mat4 transformMatrix;

    void resetTransform();
public:
    Model();
    void draw();
    void setPosition(glm::vec3);
    glm::vec3 getPosition();
    void setRotation(glm::vec3 axis);
};