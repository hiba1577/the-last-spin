#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect position;
} Background;

void initBackground(Background *bg, SDL_Renderer *renderer, const char *path);
void drawBackground(Background *bg, SDL_Renderer *renderer);
void freeBackground(Background *bg);

#endif
