#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect position;
} Button;

void initButton(Button *btn, SDL_Renderer *renderer,
                const char *path, int x, int y, int w, int h);

void drawButton(Button *btn, SDL_Renderer *renderer);

int isInside(Button *btn, int mouseX, int mouseY);

void freeButton(Button *btn);

#endif

