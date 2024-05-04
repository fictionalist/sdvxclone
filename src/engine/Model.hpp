#pragma once

#include <glm/glm.hpp>

#include "Renderable.hpp"
#include "Shader.hpp"

class Model : public Renderable {
private:
public:
    Model();
    void draw(Shader*, Texture* = nullptr);
};