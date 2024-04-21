#include "Input.hpp"

#include <cstdio>
#include <map>

#include <SDL2/SDL.h>

namespace Input {
    std::map<SDL_KeyCode, InputEnum> keybinds;

    std::map<InputEnum, bool> buttonState;
}

bool Input::init() {
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_s, InputEnum::BUTTON_A));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_d, InputEnum::BUTTON_B));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_j, InputEnum::BUTTON_C));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_k, InputEnum::BUTTON_D));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_x, InputEnum::BUTTON_FX_L));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_m, InputEnum::BUTTON_FX_R));
    keybinds.insert(std::make_pair(SDL_KeyCode::SDLK_RETURN, InputEnum::BUTTON_START));

    buttonState.insert(std::make_pair(InputEnum::BUTTON_A, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_B, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_C, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_D, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_FX_L, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_FX_R, false));
    buttonState.insert(std::make_pair(InputEnum::BUTTON_START, false));
    return true;
}

void Input::handleKeybind(SDL_Event* event) {
    for (auto& bind : keybinds) {
        if (event->key.keysym.sym == bind.first) {
            if (event->key.state == SDL_PRESSED) {
                if (event->key.repeat) {
                    continue;
                }
                buttonState[bind.second] = true;
            } else if (event->key.state == SDL_RELEASED) {
                buttonState[bind.second] = false;
            }
        }
    }
}

InputState Input::getState() {
    InputState state = {};
    state.u32 = 0;

    state.bits.buttonA     = buttonState[InputEnum::BUTTON_A];
    state.bits.buttonB     = buttonState[InputEnum::BUTTON_B];
    state.bits.buttonC     = buttonState[InputEnum::BUTTON_C];
    state.bits.buttonD     = buttonState[InputEnum::BUTTON_D];
    state.bits.buttonFXL   = buttonState[InputEnum::BUTTON_FX_L];
    state.bits.buttonFXR   = buttonState[InputEnum::BUTTON_FX_R];
    state.bits.buttonStart = buttonState[InputEnum::BUTTON_START];

    return state;
}