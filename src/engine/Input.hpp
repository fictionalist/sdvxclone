#pragma once

#include <SDL2/SDL.h>

union InputState {
    struct {
        bool buttonA, buttonB, buttonC, buttonD, buttonFXL, buttonFXR, buttonStart;
    } bits;
    unsigned int u32;
};

enum class InputEnum {
    BUTTON_A = 1 << 0,
    BUTTON_B = 1 << 1,
    BUTTON_C = 1 << 2,
    BUTTON_D = 1 << 3,
    BUTTON_FX_L = 1 << 4,
    BUTTON_FX_R = 1 << 5,
    BUTTON_START = 1 << 6
};

namespace Input {
    bool init();
    void handleKeybind(SDL_Event*);
    InputState getState();
}