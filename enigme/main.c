#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "header6.h"

#define LARGEUR  1000
#define HAUTEUR   600
#define FPS        60

int main(int argc, char* argv[]) {

    // -------------------------------------------------------
    // 1. INITIALISATION SDL
    // -------------------------------------------------------
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // -------------------------------------------------------
    // 2. FENÊTRE ET RENDERER
    // -------------------------------------------------------
    SDL_Window* window = SDL_CreateWindow(
        "Sous-Menu Enigme - Inception",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR, HAUTEUR,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        printf("Erreur renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // -------------------------------------------------------
    // 3. POLICE
    // -------------------------------------------------------
    TTF_Font* font = TTF_OpenFont("font.ttf", 22);
    if (!font) {
        font = TTF_OpenFont(
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 22);
        if (!font) {
            printf("Erreur police : %s\n", TTF_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            Mix_CloseAudio();
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }

    // -------------------------------------------------------
    // 4. INIT MENU ÉNIGME
    // -------------------------------------------------------
    MenuEnigme menu_enigme;
    enigme_init(&menu_enigme, renderer);

    // -------------------------------------------------------
    // 5. BOUCLE PRINCIPALE
    // -------------------------------------------------------
    int etat_global    = 0;  // 0=en cours  -1=quitter
    int running        = 1;
    SDL_Event event;
    Uint32 delai_frame = 1000 / FPS;

    while (running) {
        Uint32 debut = SDL_GetTicks();

        // Événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            enigme_gerer_evenements(&menu_enigme, &event, &etat_global);
        }

        if (etat_global == -1)
            running = 0;

        // Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        enigme_afficher(&menu_enigme, renderer, font);
        SDL_RenderPresent(renderer);

        // FPS
        Uint32 duree = SDL_GetTicks() - debut;
        if (duree < delai_frame)
            SDL_Delay(delai_frame - duree);
    }

    // -------------------------------------------------------
    // 6. NETTOYAGE
    // -------------------------------------------------------
    enigme_liberer(&menu_enigme);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
