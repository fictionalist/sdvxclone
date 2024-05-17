#include "Renderer.hpp"

#include <optional>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logging.hpp"
#include "Configuration.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Font.hpp"
#include "Scene.hpp"
#include "Image.hpp"
#include "Renderable.hpp"
#include "Model.hpp"
#include "UIElement.hpp"

namespace Renderer {
    std::vector<Renderable*> renderList;
    std::vector<Renderable*> zOrderedRenderList;
    std::vector<Renderable*> interfaceList;

    glm::ivec2 resolution = glm::ivec2(1280, 720);

    bool initialized = false;
    Shader* defaultShader;
    Shader* interfaceShader;
    Shader* atlasShader;
    Shader* processShader;
    
    Font* defaultFont;

    glm::mat4 interfaceProjection;
    glm::mat4 gameProjection;
    unsigned int runTime = 0;

    unsigned int multisampleFramebuffer = 0;
    unsigned int multisampleTexture = 0;

    unsigned int processFramebuffer = 0;
    unsigned int processVBO = 0;
    unsigned int processVAO = 0;
    unsigned int processTexture = 0;

    bool generatePostProcessBuffer();
}

bool Renderer::isInitialized() {
    return initialized;
}

void Renderer::resetFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, resolution.x, resolution.y);
}

bool Renderer::generatePostProcessBuffer() {
    glGenTextures(1, &multisampleTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGB, resolution.x, resolution.y, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glGenFramebuffers(1, &multisampleFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampleFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture, 0);

    unsigned int multisampleRenderbuffer;
    glGenRenderbuffers(1, &multisampleRenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multisampleRenderbuffer);

    glGenTextures(1, &processTexture);
    glBindTexture(GL_TEXTURE_2D, processTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &processFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, processFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, processTexture, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            Logging::error("Error when generating framebuffer: %d", glGetString(error));
        } 
        Logging::error("Postprocess framebuffer incomplete.");
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &processVAO);
    glGenBuffers(1, &processVBO);

    glBindVertexArray(processVAO);
    glBindBuffer(GL_ARRAY_BUFFER, processVBO);

    const float processVertices[6][4] = {
        {-1.0f, -1.0f, 0.0f, 0.0f},
        { 1.0f, -1.0f, 1.0f, 0.0f},
        { 1.0f,  1.0f, 1.0f, 1.0f},
        {-1.0f, -1.0f, 0.0f, 0.0f},
        { 1.0f,  1.0f, 1.0f, 1.0f},
        {-1.0f,  1.0f, 0.0f, 1.0f}
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(processVertices), processVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

bool Renderer::init(glm::ivec2 initialResolution) {
    Logging::info("Renderer: Initializing font library.");
    if (!Font::init()) {
        quit();
        return false;
    }

    Logging::info("Renderer: Initializing image module.");

    Image::init();

    resolution = initialResolution;

    Logging::info("Renderer: Generating post-processing buffers.");

    generatePostProcessBuffer();

    interfaceProjection = glm::ortho(0.0f, (float)resolution.x, (float)resolution.y, 0.0f, -1.0f, 1.0f);
    gameProjection = glm::perspective(glm::radians(45.0f), (float)resolution.x / (float)resolution.y, 0.1f, 500.0f);

    Logging::info("Renderer: Generating base shaders.");
    
    defaultShader = new Shader("default shader");
    defaultShader->loadShader("./data/shaders/default.vert", GL_VERTEX_SHADER);
    defaultShader->loadShader("./data/shaders/default.frag", GL_FRAGMENT_SHADER);
    defaultShader->link();
    defaultShader->use();
    defaultShader->setMat4("uProjection", gameProjection);

    interfaceShader = new Shader("interface shader");
    interfaceShader->loadShader("./data/shaders/interface.vert", GL_VERTEX_SHADER);
    interfaceShader->loadShader("./data/shaders/interface.frag", GL_FRAGMENT_SHADER);
    interfaceShader->link();
    interfaceShader->use();
    interfaceShader->setMat4("uProjection", interfaceProjection);

    atlasShader = new Shader("font atlas shader");
    atlasShader->loadShader("./data/shaders/atlas.vert", GL_VERTEX_SHADER);
    atlasShader->loadShader("./data/shaders/atlas.frag", GL_FRAGMENT_SHADER);
    atlasShader->link();

    processShader = new Shader("post processing shader");
    processShader->loadShader("./data/shaders/baseprocess.vert", GL_VERTEX_SHADER);
    processShader->loadShader("./data/shaders/baseprocess.frag", GL_FRAGMENT_SHADER);
    processShader->link();
    processShader->setInt("renderedFrame", 0);

    Logging::info("Renderer: Generating base font.");

    defaultFont = new Font("./data/fonts/NotoSans-Regular.ttf", 24);
    
    Logging::info("Renderer: Initializing default camera values.");

    Camera::init(defaultShader);
    Camera::setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    Camera::lookAt(glm::vec3(0.0f));

    Logging::info("Renderer: Setting default OpenGL values.");

    glViewport(0, 0, resolution.x, resolution.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Logging::info("Renderer: Initialized.");

    initialized = true;
    return true;
}

void Renderer::update(unsigned long long delta) {
    runTime += delta;
    defaultShader->use();
    defaultShader->setInt("uTime", runTime);
    interfaceShader->use();
    interfaceShader->setInt("uTime", runTime);
}

void Renderer::draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampleFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    Scene::currentScene->draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, processFramebuffer);
    glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(processVAO);
    glBindBuffer(GL_ARRAY_BUFFER, processVBO);
    glBindTexture(GL_TEXTURE_2D, processTexture);
    processShader->use();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::quit() {
    if (defaultShader != nullptr)
        delete defaultShader;
    if (interfaceShader != nullptr)
        delete interfaceShader;
    if (atlasShader != nullptr)
        delete atlasShader;

    if (defaultFont != nullptr)
        delete defaultFont;
    Font::quit();

    Texture::quit();

    initialized = false;
}

glm::ivec2 Renderer::getCurrentResolution() {
    return resolution;
}