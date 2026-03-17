#include <SDL2/SDL.h>
#include "menu_sauvegarde.h"

void lancerSauvegarde(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_Event e;
    int quit = 0;

    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                quit = 1;

            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                quit = 1;
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}
