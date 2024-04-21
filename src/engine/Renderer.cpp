#include "Renderer.hpp"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logging.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Font.hpp"
#include "Renderable.hpp"
#include "UIElement.hpp"

namespace Renderer {
    std::vector<Renderable*> renderList;
    std::vector<Renderable*> zOrderedRenderList;

    bool initialized = false;
    Shader* baseShader;
    Shader* interfaceShader;
    Shader* atlasShader;
    
    Font* defaultFont;

    glm::mat4 interfaceProjection;
    glm::mat4 gameProjection;
    unsigned int runTime = 0;

    unsigned int prepassFramebuffer;
}

bool Renderer::isInitialized() {
    return initialized;
}

void Renderer::resetFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1280, 720);
}

bool Renderer::init() {
    if (!Font::init()) {
        quit();
        return false;
    }

    glGenFramebuffers(1, &prepassFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, prepassFramebuffer);

    unsigned int prepassTexture;
    glGenTextures(1, &prepassTexture);
    glBindTexture(GL_TEXTURE_2D, prepassTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, prepassTexture, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            Logging::error("Error when generating framebuffer: %d", glGetString);
        } 
        Logging::error("Prepass framebuffer incomplete: %d");
    }

    interfaceProjection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
    gameProjection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    baseShader = new Shader();
    baseShader->loadShader("./data/shaders/default.vert", GL_VERTEX_SHADER);
    baseShader->loadShader("./data/shaders/default.frag", GL_FRAGMENT_SHADER);
    baseShader->link();
    baseShader->use();
    baseShader->setMat4("uProjection", gameProjection);

    interfaceShader = new Shader();
    interfaceShader->loadShader("./data/shaders/interface.vert", GL_VERTEX_SHADER);
    interfaceShader->loadShader("./data/shaders/interface.frag", GL_FRAGMENT_SHADER);
    interfaceShader->link();
    interfaceShader->use();
    interfaceShader->setMat4("uProjection", interfaceProjection);

    atlasShader = new Shader();
    atlasShader->loadShader("./data/shaders/atlas.vert", GL_VERTEX_SHADER);
    atlasShader->loadShader("./data/shaders/atlas.frag", GL_FRAGMENT_SHADER);
    atlasShader->link();

    defaultFont = new Font("./data/fonts/NotoSans-Regular.ttf", 24);

    Camera::init(baseShader);

    glViewport(0, 0, 1280, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initialized = true;
    return true;
}

void Renderer::addRenderable(Renderable* r) {
    renderList.push_back(r);
}

void Renderer::update(unsigned long long delta) {
    runTime += delta;
    baseShader->use();
    baseShader->setInt("uTime", runTime);
    interfaceShader->use();
    interfaceShader->setInt("uTime", runTime);
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i : renderList) {
        i->draw();
    }
}

void Renderer::quit() {
    if (baseShader != nullptr)
        delete baseShader;
    if (interfaceShader != nullptr)
        delete interfaceShader;
    if (atlasShader != nullptr)
        delete atlasShader;

    if (defaultFont != nullptr)
        delete defaultFont;
    Font::quit();

    for (auto i : renderList) {
        delete i;
    }
    renderList.clear();

    Texture::quit();

    initialized = false;
}