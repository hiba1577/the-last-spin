#ifndef MENU_H
#define MENU_H

#include "background.h"
#include "button.h"

typedef struct
{
    Background bg;

    Button mono;
    Button multi;
    Button retour;

    Button avatar1;
    Button avatar2;
    Button input1;
    Button input2;
    Button valider;

    int running;
    int state;   // 0 = first screen, 1 = selection screen

} Menu;

void initMenu(Menu *m, SDL_Renderer *renderer);
void handleInput(Menu *m, SDL_Event *event);
void drawMenu(Menu *m, SDL_Renderer *renderer);
void freeMenu(Menu *m);

#endif

