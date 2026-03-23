#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME   20
#define MAX_SCORES 100
#define FONT_PATH  "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"

typedef struct {
    char name[MAX_NAME];
    int  score;
} Player;

typedef struct {
    SDL_Texture *background;
    SDL_Texture *btn_valider;
    SDL_Texture *btn_retour;
    SDL_Texture *btn_quit;
    SDL_Texture *star1;
    SDL_Texture *star2;
    SDL_Texture *star3;

    SDL_Rect rect_valider;
    SDL_Rect rect_retour;
    SDL_Rect rect_quit;
    SDL_Rect rect_star[3];

    TTF_Font  *font;
    Mix_Music *victory;
    Mix_Chunk *hover_sound;

    Player top3[3];
    int current_score; /* à affecter avant d'appeler handleScoreMenu */
} ScoreMenu;

/* scores */
void sortScores(Player tab[], int n);
void saveScore(Player p);
void loadTop3(Player top3[]);

/* menu */
int  initScoreMenu(ScoreMenu *menu, SDL_Renderer *renderer);
void handleScoreMenu(ScoreMenu *menu, SDL_Renderer *renderer,
                     SDL_Window *window, int *quit_game);
void freeScoreMenu(ScoreMenu *menu);

#endif
