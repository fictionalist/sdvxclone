#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"

namespace Camera {
    void init(Shader* baseGameShader);
    void setPosition(glm::vec3 position);
    void lookAt(glm::vec3 target);
}