#ifndef OPTION_H
#define OPTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    // Images
    SDL_Texture *background;

    SDL_Texture *aug1, *aug2;
    SDL_Texture *dim1, *dim2;
    SDL_Texture *full1, *full2;
    SDL_Texture *norm1, *norm2;
    SDL_Texture *ret1,  *ret2;

    // Rectangles boutons
    SDL_Rect rAug, rDim;
    SDL_Rect rFull, rNorm;
    SDL_Rect rRet;

    // -------- TEXTE --------
    TTF_Font *font;

    SDL_Texture *txtVolume;
    SDL_Texture *txtMode;

    SDL_Rect rTxtVolume;
    SDL_Rect rTxtMode;
    // -----------------------

    // Son
    int volume;
    Mix_Music *music;
    Mix_Chunk *hover;
    int hoverPlayed;

} OptionMenu;


// Fonctions
void lancerOption(SDL_Window *window, SDL_Renderer *renderer);
int initOption(OptionMenu *opt, SDL_Renderer *renderer);
void handleOptionEvents(OptionMenu *opt, SDL_Event *e,
                        SDL_Window *window, int *running);
void renderOption(OptionMenu *opt, SDL_Renderer *renderer);
void freeOption(OptionMenu *opt);

#endif
