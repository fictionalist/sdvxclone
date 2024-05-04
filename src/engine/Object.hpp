#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Model.hpp"
#include "Texture.hpp"

class Object {
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::mat4 transformMatrix;
    Shader* shader;
    Texture* texture;
    glm::vec4 color;
    bool visible;

    void resetTransform();
public:
    Object();
    ~Object();

    void setShader(Shader*);
    void update(unsigned int deltaTick);
    void draw();

    Model* model;

    void setPosition(glm::vec3);
    glm::vec3 getPosition();
    void setRotation(glm::vec3 axisAngles);
    void setTexture(Texture*);
    void setColor(glm::vec4);
    void setVisibility(bool);
};