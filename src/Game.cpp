#include "Game.hpp"

#include <cstdio>

#include <windows.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "engine/Logging.hpp"
#include "engine/LuaScript.hpp"
#include "engine/Configuration.hpp"
#include "engine/Input.hpp"
#include "engine/Renderer.hpp"
#include "engine/Scene.hpp"

namespace Game {
    static SDL_Window* gameWindow = nullptr;
    static SDL_GLContext glContext = nullptr;
    static bool running = false;

    void handleEvent(SDL_Event*);
}

bool Game::init() {
    printf("Starting up...\n");

    if (!Logging::init()) {
        return false;
    }

    if (!Configuration::init()) {
        return false;
    }

    if (!LuaScript::init()) {
        return false;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    Input::init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    gameWindow = SDL_CreateWindow("SDVX Clone - frames per second: 0.00",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL);

    glContext = SDL_GL_CreateContext(gameWindow);

    glewExperimental = GL_TRUE;
    GLenum glError = glewInit();
    if (glError != GLEW_OK) {
        printf("Failed to initialize GLEW: %s\n", glewGetErrorString(glError));
        return false;
    }

    if (!Renderer::init()) {
        printf("Failed to initialize Renderer.\n");
        Game::quit();
        return false;
    }

    Scene::setScene(SceneList::Gameplay);
    
    running = true;

    return true;
}

void Game::handleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (event->key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    running = false;
                    break;
                default:
                    Input::handleKeybind(event);
                    break;
            }
            break;
        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void Game::loop() {
    SDL_Event event;

    unsigned long long renderFrameStart, renderFrameEnd;
    double renderFrameTime = 0.0;
    renderFrameStart = renderFrameEnd = SDL_GetPerformanceCounter();
    unsigned int frameCount = 0;
    char buf[128];

    unsigned long long lastTick, currentTick;
    lastTick = currentTick = SDL_GetPerformanceCounter();
    unsigned long long runTime = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }
        currentTick = SDL_GetPerformanceCounter();
        unsigned long long tickDelta = ((currentTick - lastTick) * 1000) / SDL_GetPerformanceFrequency();
        runTime += tickDelta;
        Scene::currentScene->update(tickDelta);
        Renderer::update((((currentTick - lastTick) * 1000) / SDL_GetPerformanceFrequency()));
        lastTick = currentTick;
        Renderer::draw();
        SDL_GL_SwapWindow(gameWindow);
        frameCount++;
        renderFrameEnd = SDL_GetPerformanceCounter();
        renderFrameTime = (double)((renderFrameEnd - renderFrameStart) * 1000 / SDL_GetPerformanceFrequency());
        if (renderFrameTime >= 1000.0) {
            snprintf(buf, 64, "SDVX Clone - frames per second: %.2f - %d - %llu", ((float)frameCount * 1000.0 / renderFrameTime), tickDelta, runTime);
            SDL_SetWindowTitle(gameWindow, buf);
            renderFrameStart = renderFrameEnd - (renderFrameTime - 1000.0);
            frameCount = 0;
        }
    }
}

void Game::quit() {
    if (Renderer::isInitialized()) {
        Renderer::quit();
    }
    if (glContext != NULL) {
        SDL_GL_DeleteContext(glContext);
    }
    if (gameWindow != NULL) {
        SDL_DestroyWindow(gameWindow);
    }
}