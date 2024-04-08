#include "Game.hpp"

#undef main

int main(int argc, char* argv[]) {
    if (!Game::init()) {
        return 1;
    }
    Game::loop();
    Game::quit();
    return 0;
}
