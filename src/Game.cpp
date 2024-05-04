#include "Game.hpp"

#include <cstdio>

#include <windows.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "engine/Logging.hpp"
#include "engine/LuaScript.hpp"
#include "engine/Configuration.hpp"
#include "engine/Input.hpp"
#include "engine/Audio.hpp"
#include "engine/Renderer.hpp"
#include "engine/Scene.hpp"

namespace Game {
    static SDL_Window* gameWindow = nullptr;
    static SDL_GLContext glContext = nullptr;
    static bool running = false;

    void handleEvent(SDL_Event*);
}

bool Game::init() {
    Logging::info("Game starting up.");

    if (!Logging::init()) {
        return false;
    }

    if (!Configuration::init()) {
        return false;
    }

    if (!LuaScript::init()) {
        return false;
    }

    if (!Audio::init()) {
        return false;
    }
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        Logging::error("Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    Input::init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    std::string resolutionString = Configuration::settings["graphics"]["resolution"].get<std::string>();

    glm::ivec2 resolution = glm::ivec2(1280, 720);
    if (!resolutionString.empty()) {
        if (unsigned int position = resolutionString.find('x')) {
            glm::ivec2 tempResolution = resolution;
            tempResolution.x = atoi(resolutionString.substr(0, position).c_str());
            tempResolution.y = atoi(resolutionString.substr(position + 1).c_str());

            printf("%d x %d\n", tempResolution.x, tempResolution.y);
            resolution = tempResolution;
        }
    }
    
    unsigned int windowMode = 0; // normally windowed
    std::string windowModeString = Configuration::settings["graphics"]["windowMode"].get<std::string>();

    if (windowModeString.compare("windowed") == 0) {
        windowMode = 0;
    } else if (windowModeString.compare("fullscreen")) {
        windowMode = SDL_WINDOW_FULLSCREEN_DESKTOP;
    } else if (windowModeString.compare("borderless")) {
        windowMode = SDL_WINDOW_FULLSCREEN;
    }

    gameWindow = SDL_CreateWindow("SDVX Clone - frames per second: 0.00",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        resolution.x, resolution.y,
        SDL_WINDOW_OPENGL | windowMode);

    glContext = SDL_GL_CreateContext(gameWindow);

    glewExperimental = GL_TRUE;
    GLenum glError = glewInit();
    if (glError != GLEW_OK) {
        Logging::error("Failed to initialize GLEW: %s\n", glewGetErrorString(glError));
        return false;
    }

    if (!Renderer::init(resolution)) {
        Logging::error("Failed to initialize Renderer.\n");
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
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            Input::handleGamepad(event);
            break;
        case SDL_CONTROLLERDEVICEADDED:
            Input::addGamepad(event);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            Input::removeGamepad(event);
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
    unsigned long long updateFrameStart, updateFrameEnd;
    double updateFrameTime = 0.0;
    updateFrameStart = updateFrameEnd = SDL_GetPerformanceCounter();
    unsigned int frameCount = 0;
    char buf[128];

    unsigned long long lastTick, currentTick;
    lastTick = currentTick = SDL_GetPerformanceCounter();
    unsigned long long runTime = 0;
    unsigned int cycleTime = 0;

    while (running) {
        currentTick = SDL_GetPerformanceCounter();
        unsigned long long tickDelta = ((currentTick - lastTick) * 1000) / SDL_GetPerformanceFrequency();
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }
        runTime += tickDelta;
        cycleTime += tickDelta;
        updateFrameStart = SDL_GetPerformanceCounter();
        Scene::currentScene->update(tickDelta);
        Renderer::update(tickDelta);
        updateFrameEnd = SDL_GetPerformanceCounter();
        renderFrameStart = SDL_GetPerformanceCounter();
        Renderer::draw();
        SDL_GL_SwapWindow(gameWindow);
        renderFrameEnd = SDL_GetPerformanceCounter();

        updateFrameTime = (double)((updateFrameTime) * 1000.0 / SDL_GetPerformanceFrequency());
        renderFrameTime = (double)((renderFrameTime) * 1000.0 / SDL_GetPerformanceFrequency());

        if (cycleTime >= 1000) {
            snprintf(buf, 64, "SDVX Clone - %dms/render - %dms/update - runtime: %llu", updateFrameTime, renderFrameTime, runTime);
            SDL_SetWindowTitle(gameWindow, buf);
            cycleTime -= 1000;
        }

        lastTick = currentTick;
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