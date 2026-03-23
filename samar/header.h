#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

/* Fenêtre et rendu */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} WindowData;

/* Images et texte */
typedef struct {
    SDL_Texture *background;
    SDL_Texture *text;
    SDL_Rect bgPos;
    SDL_Rect textPos;
    TTF_Font *font;
    int showText;
} Media;

/* Audio */
typedef struct {
    Mix_Chunk *shortSound;
    Mix_Music *music;
    int musicPlaying;
} Audio;

/* Prototypes */
int initSystem(WindowData *wd);
int loadMedia(WindowData *wd, Media *m);
int loadAudio(Audio *a);
void events(WindowData *wd, Media *m, Audio *a, int *run);
void render(WindowData *wd, Media *m);
void cleanup(WindowData *wd, Media *m, Audio *a);

#endif
