#include "score.h"

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    { printf("SDL_Init: %s\n", SDL_GetError()); return 1; }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *window = SDL_CreateWindow(
        "Meilleurs Scores",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED);

    ScoreMenu menu;
    if (!initScoreMenu(&menu, renderer))
    {
        printf("Echec initScoreMenu\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio(); TTF_Quit(); IMG_Quit(); SDL_Quit();
        return 1;
    }

    /* score du joueur courant (à remplacer par le vrai score de ta partie) */
    menu.current_score = 420;

    int quit = 0;
    handleScoreMenu(&menu, renderer, window, &quit);

    freeScoreMenu(&menu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
