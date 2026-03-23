#include "background.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

void initBackground(Background *bg, SDL_Renderer *renderer, const char *path)
{
    bg->texture = NULL;   // IMPORTANT

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("Background load error: %s\n", IMG_GetError());
        return;
    }

    bg->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!bg->texture)
        printf("Texture creation error: %s\n", SDL_GetError());

    bg->position = (SDL_Rect){0,0,0,0};
}

void drawBackground(Background *bg, SDL_Renderer *renderer)
{
    if (bg->texture)
        SDL_RenderCopy(renderer, bg->texture, NULL, NULL);
}

void freeBackground(Background *bg)
{
    if (bg->texture)
        SDL_DestroyTexture(bg->texture);
}

