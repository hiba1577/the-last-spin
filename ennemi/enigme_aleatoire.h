#ifndef ENIGME_ALEATOIRE_H
#define ENIGME_ALEATOIRE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===== STRUCTURE ENIGME =====
typedef struct {
    // Texte
    char question[256];
    char reponse1[128];
    char reponse2[128];
    char reponse3[128];

    // Numéros
    int numQuestSelect;  // numéro question choisie aléatoirement
    int numbr;           // numéro bonne réponse (1, 2 ou 3)

    // Textures
    SDL_Texture *tex_question;
    SDL_Texture *tex_rep1;
    SDL_Texture *tex_rep2;
    SDL_Texture *tex_rep3;

    // Positions
    SDL_Rect pos_question;
    SDL_Rect pos_rep1;
    SDL_Rect pos_rep2;
    SDL_Rect pos_rep3;

    // Couleur et police (texte)
    SDL_Color couleur;
    TTF_Font *font;

    // État : -1=echec  0=non résolu  1=succes
    int etat;

} Enigme;

// ===== FONCTIONS =====
int    compterLignes (const char *fichier);
Enigme decomposer    (const char *ligne, SDL_Renderer *r, TTF_Font *f);
Enigme generer       (const char *fichier, SDL_Renderer *r, TTF_Font *f);
void   afficherEnigme(Enigme *e, SDL_Renderer *r);
void   libererEnigme (Enigme *e);

#endif
