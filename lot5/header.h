#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ================================================
   CONSTANTES
   ================================================ */
#define MAX_QUESTIONS  50    // max questions fichier
#define MAX_CHAR      256    // max caracteres
#define DUREE_ENIGME   30    // secondes pour repondre

/* ================================================
   ETATS ENIGME
   ================================================ */
typedef enum {
    ENIGME_EN_COURS,     // pas encore repondu
    ENIGME_SUCCES,       // bonne reponse
    ENIGME_ECHEC         // mauvaise ou timeout
} EtatEnigme;

/* ================================================
   STRUCTURE ENIGME
   ================================================ */
typedef struct {
    /* TACHE 1 : contenu fichier */
    char question[MAX_CHAR];   // texte question
    char reponse1[MAX_CHAR];   // texte reponse 1
    char reponse2[MAX_CHAR];   // texte reponse 2
    char reponse3[MAX_CHAR];   // texte reponse 3
    int  bonne_reponse;        // 1, 2 ou 3

    /* TACHE 2 : numero aleatoire */
    int  numQuestSelect;       // numero question

    /* textures SDL */
    SDL_Texture *tex_question; // texture question
    SDL_Texture *tex_rep1;     // texture reponse 1
    SDL_Texture *tex_rep2;     // texture reponse 2
    SDL_Texture *tex_rep3;     // texture reponse 3

    /* positions ecran */
    SDL_Rect pos_question;
    SDL_Rect pos_rep1;
    SDL_Rect pos_rep2;
    SDL_Rect pos_rep3;

    /* police */
    TTF_Font  *font;
    SDL_Color  couleur;

    /* TACHE 3 : etat */
    EtatEnigme etat;

    /* TACHE 4 : score vies level */
    int score;
    int vies;
    int level;

    /* TACHE 5 : chronometre */
    Uint32 temps_debut;
    int    duree;
    float  angle_anim;

} Enigme;

/* ================================================
   PROTOTYPES
   ================================================ */

/* TACHE 1 */
void   remplirFichier  (const char *fichier);
int    compterLignes   (const char *fichier);
Enigme chargerEnigme   (const char *ligne,
                        SDL_Renderer *r, TTF_Font *f,
                        int score, int vies, int level);

/* TACHE 2 */
Enigme genererEnigme   (const char *fichier,
                        SDL_Renderer *r, TTF_Font *f,
                        int *derniere,
                        int score, int vies, int level);

/* TACHE 3 */
void   resoudreEnigme  (Enigme *e, int rep_joueur);
void   afficherEnigme  (Enigme *e, SDL_Renderer *r);

/* TACHE 4 */
void   afficherHUD     (Enigme *e, SDL_Renderer *r);

/* TACHE 5 */
int    timerEcoule     (Enigme *e);
void   afficherChrono  (Enigme *e, SDL_Renderer *r);

/* liberation */
void   libererEnigme   (Enigme *e);

#endif
