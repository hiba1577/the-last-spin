#ifndef HEADER6_H
#define HEADER6_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// ===== ÉTATS DU SOUS-MENU ÉNIGME =====
typedef enum {
    ENIGME_CHOIX,
    ENIGME_QUIZ,
    ENIGME_RESULTAT
} EtatEnigme;

// ===== STRUCTURE QUESTION =====
typedef struct {
    char question[256];
    char choix[3][128];
    int bonne_reponse; // 0=A  1=B  2=C
} Question;

// ===== STRUCTURE BOUTON =====
typedef struct {
    SDL_Rect rect;
    char texte[128];
    int survole;
    int actif;
} Bouton;

// ===== DONNÉES DU MENU ÉNIGME =====
typedef struct {
    // Assets
    SDL_Texture* fond;
    Mix_Music*   musique_suspense;
    Mix_Chunk*   son_hover;

    // Boutons écran choix
    Bouton btn_quiz;
    Bouton btn_puzzle;

    // Boutons écran quiz
    Bouton btn_A;
    Bouton btn_B;
    Bouton btn_C;

    // Quiz — 6 questions thème Inception
    Question questions[6];
    int question_courante;
    int reponse_choisie;
    int musique_lancee;

    // Score
    int score;

    // État interne
    EtatEnigme etat;

    // Résultat
    char msg_resultat[64];
    Uint32 temps_resultat;

} MenuEnigme;

// ===== FONCTIONS =====
void enigme_init(MenuEnigme* m, SDL_Renderer* renderer);
void enigme_gerer_evenements(MenuEnigme* m, SDL_Event* e, int* etat_global);
void enigme_afficher(MenuEnigme* m, SDL_Renderer* renderer, TTF_Font* font);
void enigme_liberer(MenuEnigme* m);
void dessiner_bouton(SDL_Renderer* r, Bouton* b, TTF_Font* f,
                     SDL_Color couleur_normal,
                     SDL_Color couleur_survol,
                     SDL_Color couleur_texte);

#endif
