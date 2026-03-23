#include "headerr.h"
#include <stdio.h>

// ================= Initialisation =================
int init(SDL_Window** window, SDL_Renderer** renderer)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 0;
    }

    if(!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG))
    {
        printf("Erreur IMG : %s\n", IMG_GetError());
        return 0;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Erreur Mix : %s\n", Mix_GetError());
        return 0;
    }

    if(TTF_Init() == -1)
    {
        printf("Erreur TTF : %s\n", TTF_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("Sous Menu Sauvegarde",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               WIDTH, HEIGHT, 0);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    return 1;
}

// ================= Texte =================
void renderText(SDL_Renderer* renderer, TTF_Font* font,
                const char* text, int x, int y)
{
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* surface =
        TTF_RenderText_Blended(font, text, color);

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// ================= Bouton =================
void drawButton(SDL_Renderer* renderer,
                TTF_Font* font,
                Button* btn)
{
    if(btn->hover)
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);

    SDL_RenderFillRect(renderer, &btn->rect);

    renderText(renderer, font, btn->text,
               btn->rect.x + 10,
               btn->rect.y + 10);
}

// ================= Vérification souris =================
bool isInside(Button* btn, int x, int y)
{
    return (x >= btn->rect.x &&
            x <= btn->rect.x + btn->rect.w &&
            y >= btn->rect.y &&
            y <= btn->rect.y + btn->rect.h);
}

// ================= Nettoyage =================
void cleanup(SDL_Window* window,
             SDL_Renderer* renderer)
{
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

