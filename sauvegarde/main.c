#include "header.h"
#include <stdio.h>

int main()
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if(!init(&window, &renderer))
        return 1;

    // ===== Chargement ressources =====
    SDL_Texture* background =
        IMG_LoadTexture(renderer, "background2.jpg");

    Mix_Music* music =
        Mix_LoadMUS("music.mp3");

    Mix_Chunk* hoverSound =
        Mix_LoadWAV("hover.wav");

    TTF_Font* font =
        TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);

    Mix_PlayMusic(music, -1);

    // ===== Boutons =====
    Button btnOui = {{300, 250, 200, 50}, "Oui", false};
    Button btnNon = {{300, 320, 200, 50}, "Non", false};
    Button btnCharger = {{300, 250, 200, 50}, "Charger le jeu", false};
    Button btnNew = {{300, 320, 200, 50}, "Nouvelle partie", false};

    bool running = true;
    bool showSecondMenu = false;
    SDL_Event event;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;

            if(event.type == SDL_MOUSEMOTION)
            {
                int mx = event.motion.x;
                int my = event.motion.y;

                Button* buttons[] =
                {&btnOui, &btnNon, &btnCharger, &btnNew};

                for(int i = 0; i < 4; i++)
                {
                    if((!showSecondMenu && i < 2) ||
                       (showSecondMenu && i >= 2))
                    {
                        if(isInside(buttons[i], mx, my))
                        {
                            if(!buttons[i]->hover)
                                Mix_PlayChannel(-1, hoverSound, 0);

                            buttons[i]->hover = true;
                        }
                        else
                            buttons[i]->hover = false;
                    }
                }
            }

            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = event.button.x;
                int my = event.button.y;

                if(!showSecondMenu &&
                   isInside(&btnOui, mx, my))
                    showSecondMenu = true;

                if(showSecondMenu &&
                   isInside(&btnNew, mx, my))
                    printf("Sous menu joueur\n");
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        renderText(renderer, font,
                   "Voulez-vous sauvegarder votre jeu ?",
                   200, 150);

        if(!showSecondMenu)
        {
            drawButton(renderer, font, &btnOui);
            drawButton(renderer, font, &btnNon);
        }
        else
        {
            drawButton(renderer, font, &btnCharger);
            drawButton(renderer, font, &btnNew);
        }

        SDL_RenderPresent(renderer);
    }

    // ===== Libération mémoire =====
    Mix_FreeMusic(music);
    Mix_FreeChunk(hoverSound);
    SDL_DestroyTexture(background);
    TTF_CloseFont(font);

    cleanup(window, renderer);

    return 0;
}

