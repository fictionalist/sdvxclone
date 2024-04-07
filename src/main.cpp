#include <cstdio>

#include <SDL2\SDL.h>

#undef main

int main(int argc, char* argv[]) {
    printf("Hello world.\n");
    
    SDL_Window* window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SDVX Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

    while (1);

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
