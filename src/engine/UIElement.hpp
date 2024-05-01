#pragma once

#include "Renderable.hpp"

class UIElement : public Renderable {
private:
    glm::ivec2 position;
    float rotation;
    glm::ivec2 size;
    glm::ivec2 origin;
    glm::mat4 transformMatrix;

    void resetTransform();

public:
    UIElement();
    void draw();
    void setSize(glm::ivec2);
    void setPosition(glm::ivec2);
    void setOrigin(glm::ivec2);
    void setRotation(float);
};