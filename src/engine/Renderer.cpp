#include "Renderer.hpp"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logging.hpp"
#include "Shader.hpp"
#include "Font.hpp"
#include "Renderable.hpp"

namespace Renderer {
    Shader* baseShader;
    Font* defaultFont;
    glm::mat4 interfaceProjection;
    glm::mat4 gameProjection;
    std::vector<Renderable*> renderList;
    bool initialized;
}

bool Renderer::isInitialized() {
    return initialized;
}

bool Renderer::init() {
    initialized = false;

    if (!Font::init()) {
        quit();
        return false;
    }

    interfaceProjection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
    gameProjection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    baseShader = new Shader();
    baseShader->loadShader("./data/shaders/default.vert", GL_VERTEX_SHADER);
    baseShader->loadShader("./data/shaders/default.frag", GL_FRAGMENT_SHADER);
    baseShader->link();
    baseShader->use();
    baseShader->setMat4("uProjection", interfaceProjection);

    Renderable* r = new Renderable();
    r->setShader(baseShader);
    renderList.push_back(r);

    glViewport(0, 0, 1280, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initialized = true;
    return true;
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i : renderList) {
        i->draw();   
    }
}

void Renderer::quit() {
    if (baseShader != nullptr) {
        delete baseShader;
    }
    if (defaultFont != nullptr) {
        delete defaultFont;
    }
    for (auto i : renderList) {
        delete i;
    }
    renderList.clear();
    initialized = false;
}