#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Font.hpp"
#include "Renderable.hpp"

namespace Renderer {
    bool init();
    void draw();
    void update(unsigned long long delta);
    void quit();

    bool isInitialized();
    void addRenderable(Renderable*);
    void resetFramebuffer();

    extern Shader* baseShader;
    extern Shader* interfaceShader;
    extern Shader* atlasShader;
    
    extern Font* defaultFont;

    extern glm::mat4 interfaceProjection;
    extern glm::mat4 gameProjection;
}
