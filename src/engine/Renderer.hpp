#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Font.hpp"
#include "Renderable.hpp"

namespace Renderer {
    bool init(glm::ivec2 initialResolution);
    void draw();
    void update(unsigned long long delta);
    void quit();

    bool isInitialized();
    void addRenderable(Renderable*);
    void addInterface(Renderable*);
    
    void resizeWindow(glm::ivec2);
    void resetFramebuffer();

    extern Shader* defaultShader;
    extern Shader* interfaceShader;
    extern Shader* atlasShader;
    
    extern Font* defaultFont;

    extern glm::mat4 interfaceProjection;
    extern glm::mat4 gameProjection;

    glm::ivec2 getCurrentResolution();
}
