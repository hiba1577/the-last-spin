#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "option.h"
#include <SDL2/SDL_ttf.h>
int main()
{
    SDL_Window *window = NULL;      // Fenêtre
    SDL_Renderer *renderer = NULL;  // Renderer
     TTF_Init();
    // Initialisation SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // Création fenêtre
    window = SDL_CreateWindow("Menu Option",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800,600,
                              SDL_WINDOW_SHOWN);

    // Création renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialisation menu option
    OptionMenu opt;
    initOption(&opt, renderer);

    int running = 1;    // Boucle principale
    SDL_Event e;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            handleOptionEvents(&opt, &e, window, &running);
        }

        SDL_RenderClear(renderer);
        renderOption(&opt, renderer);
        SDL_RenderPresent(renderer);
    }

    // Libération mémoire
    freeOption(&opt);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    return 0;
}

