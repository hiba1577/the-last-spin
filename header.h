#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

/* ============================================================
   ÉTATS DU JEU  ← doit être ici avant tout prototype
   ============================================================ */
typedef enum {
    ETAT_MENU_PRINCIPAL,
    ETAT_MENU_OPTION,
    ETAT_MENU_SAUVEGARDE,
    ETAT_MENU_JOUEUR,
    ETAT_MENU_SCORE,
    ETAT_MENU_ENIGME,
    ETAT_QUITTER
} EtatJeu;

/* ============================================================
   PROTOTYPES
   ============================================================ */
int          init              (SDL_Window **window, SDL_Renderer **renderer);
SDL_Texture* loadTexture       (const char *file, SDL_Renderer *renderer);
int          isMouseOver       (SDL_Rect rect);
void         cleanUp           (SDL_Window *window, SDL_Renderer *renderer);
EtatJeu      lancerMenuPrincipal(SDL_Window *w, SDL_Renderer *r,
                                  Mix_Music *music, int *quit);

#endif
