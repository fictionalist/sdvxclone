#include "Renderer.hpp"

#include <GL/glew.h>

#include "Shader.hpp"

namespace Renderer {
    Shader* baseShader;
}

bool Renderer::init() {
    baseShader = new Shader();
    baseShader->loadShader("./data/shaders/default.vert", GL_VERTEX_SHADER);
    baseShader->loadShader("./data/shaders/default.frag", GL_FRAGMENT_SHADER);
    baseShader->link();
    baseShader->use();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return true;
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
