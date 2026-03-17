#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "background.h"

typedef struct {
    SDL_Texture *texture;
    SDL_Rect     position;
} MenuButton;

typedef struct {
    Background bg;
    MenuButton mono;
    MenuButton multi;
    MenuButton retour;
    MenuButton avatar1;
    MenuButton avatar2;
    MenuButton input1;
    MenuButton input2;
    MenuButton valider;
    int running;
    int state;
} Menu;

void initMenu        (Menu *m, SDL_Renderer *renderer);
void handleMenuInput (Menu *m, SDL_Event *event);
void drawMenu        (Menu *m, SDL_Renderer *renderer);
void freeMenu        (Menu *m);
void lancerMenuJoueur(Menu *m, SDL_Renderer *renderer); // NOUVEAU
int  menuButtonClicked(MenuButton *btn, int mx, int my);

#endif
