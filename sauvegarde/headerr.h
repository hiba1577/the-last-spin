#ifndef HEADERR_H
#define HEADERR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

// Structure bouton
typedef struct {
    SDL_Rect rect;
    const char* text;
    bool hover;
} Button;

// Initialisation
int init(SDL_Window** window, SDL_Renderer** renderer);

// Texte
void renderText(SDL_Renderer* renderer, TTF_Font* font,
                const char* text, int x, int y);

// Dessin bouton
void drawButton(SDL_Renderer* renderer, TTF_Font* font,
                Button* btn);

// Vérifier si souris dans bouton
bool isInside(Button* btn, int x, int y);

// Nettoyage
void cleanup(SDL_Window* window, SDL_Renderer* renderer);

#endif

