#include "button.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

void initButton(Button *btn, SDL_Renderer *renderer,
                const char *path, int x, int y, int w, int h)
{
    // Always initialize pointer
    btn->texture = NULL;

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("Button load error (%s): %s\n", path, IMG_GetError());
        return;
    }

    btn->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!btn->texture)
    {
        printf("Texture creation error: %s\n", SDL_GetError());
        return;
    }

    // Set button position and size
    btn->position.x = x;
    btn->position.y = y;
    btn->position.w = w;
    btn->position.h = h;
}

void drawButton(Button *btn, SDL_Renderer *renderer)
{
    if (btn->texture != NULL)
    {
        SDL_RenderCopy(renderer, btn->texture, NULL, &btn->position);
    }
}

int isInside(Button *btn, int mouseX, int mouseY)
{
    if (mouseX >= btn->position.x &&
        mouseX <= btn->position.x + btn->position.w &&
        mouseY >= btn->position.y &&
        mouseY <= btn->position.y + btn->position.h)
    {
        return 1;
    }

    return 0;
}

void freeButton(Button *btn)
{
    if (btn->texture != NULL)
    {
        SDL_DestroyTexture(btn->texture);
        btn->texture = NULL;
    }
}

