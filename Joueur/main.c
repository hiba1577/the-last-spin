#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "menu.h"

#define WIDTH 1280
#define HEIGHT 720

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Sous Menu Joueur",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);

    Menu menu;
    initMenu(&menu, renderer);

    SDL_Event event;

    while (menu.running)
    {
        while (SDL_PollEvent(&event))
            handleInput(&menu, &event);

        SDL_RenderClear(renderer);
        drawMenu(&menu, renderer);
        SDL_RenderPresent(renderer);
    }

    freeMenu(&menu);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}

