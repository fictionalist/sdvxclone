#include "Game.hpp"

#include <cstdio>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "engine/Logging.hpp"
#include "engine/Renderer.hpp"

namespace Game {
    static SDL_Window* gameWindow = NULL;
    static SDL_GLContext glContext = NULL;
    static bool running = false;

    void handleEvent(SDL_Event*);
}

bool Game::init() {
    printf("Starting up...\n");

    if (!Logging::init()) {
        return false;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    gameWindow = SDL_CreateWindow("SDVX Clone",
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
    
    running = true;

    return true;
}

void Game::handleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void Game::loop() {
    SDL_Event event;

    unsigned long long frameCurrent, frameLast;
    unsigned long long renderFrameStart, renderFrameEnd;
    frameCurrent = frameLast = SDL_GetPerformanceCounter();
    double renderFrameTime = 0.0;
    renderFrameStart = renderFrameEnd = SDL_GetPerformanceCounter();
    double frameTime = 0.0;
    char buf[128];

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }
        Renderer::draw();
        SDL_GL_SwapWindow(gameWindow);
        renderFrameEnd = SDL_GetPerformanceCounter();
        frameCurrent = renderFrameEnd;
        renderFrameTime = (double)((renderFrameEnd - renderFrameStart) * 1000 / SDL_GetPerformanceFrequency());

        snprintf(buf, 64, "SDVX Clone - render frame time: %.2fms", renderFrameTime);
        SDL_SetWindowTitle(gameWindow, buf);
        renderFrameStart = renderFrameEnd;
    }
}

void Game::quit() {
    if (glContext != NULL) {
        SDL_GL_DeleteContext(glContext);
    }
    if (gameWindow != NULL) {
        SDL_DestroyWindow(gameWindow);
    }
}