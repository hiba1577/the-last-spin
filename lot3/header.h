#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>      // SDL principal
#include <SDL2/SDL_image.h>// charger images
#include <SDL2/SDL_ttf.h>  // polices texte
#include <stdio.h>         // printf
#include <stdlib.h>        // rand srand
#include <math.h>          // sin pour zigzag
#include <time.h>          // time pour rand

/* ================================================
   ETATS - ce que peut etre chaque personnage
   ================================================ */

/* 3 etats possibles pour l'ennemi */
typedef enum {
    ENNEMI_VIVANT,        // vie pleine → couleur normale
    ENNEMI_BLESSE,        // vie a moitie → teinte rouge
    ENNEMI_NEUTRALISE     // vie = 0 → disparait
} EtatEnnemi;

/* 2 animations possibles pour l'ennemi */
typedef enum {
    ANIM_DEPLACEMENT,     // ennemi marche
    ANIM_ATTAQUE          // ennemi attaque
} AnimEnnemi;

/* 2 trajectoires selon le level */
typedef enum {
    TRAJECTOIRE_LINEAIRE, // level 1 : gauche droite
    TRAJECTOIRE_ZIGZAG    // level 2 : ondulation
} TypeTrajectoire;

/* 3 etats possibles pour le joueur */
typedef enum {
    JOUEUR_VIVANT,        // joueur en vie
    JOUEUR_BLESSE,        // joueur touche
    JOUEUR_MORT           // joueur mort
} EtatJoueur;

/* ================================================
   STRUCTURES - les donnees de chaque objet
   ================================================ */

/* boite invisible autour de chaque personnage
   utilisee pour detecter les collisions */
typedef struct {
    int x, y;             // coin haut gauche
    int w, h;             // largeur et hauteur
} BoundingBox;

/* element de scene = objet que le joueur peut ramasser */
typedef struct {
    float        x, y;    // position a l'ecran
    int          largeur; // largeur en pixels
    int          hauteur; // hauteur en pixels
    SDL_Texture *texture; // image de l'objet
    BoundingBox  bb;      // boite collision
    int          actif;   // 1=visible 0=ramasse
} ES;

/* toutes les donnees de l'ennemi */
typedef struct {
    float           x, y;           // position ecran
    float           vx, vy;         // vitesse deplacement
    int             direction;       // 1=droite -1=gauche
    int             largeur;         // taille affichage
    int             hauteur;
    int             vie;             // vie actuelle (0 a 3)
    int             vie_max;         // vie maximum
    EtatEnnemi      etat;            // vivant blesse mort
    SDL_Texture    *spritesheet;     // image avec toutes les frames
    SDL_Rect        frames[12];      // decoupage de la spritesheet
    int             nb_frames;       // combien de frames
    int             frame_courante;  // quelle frame afficher
    Uint32          temps_anim;      // timer pour changer frame
    int             delai_anim;      // ms entre chaque frame
    AnimEnnemi      anim_courante;   // marche ou attaque
    TypeTrajectoire trajectoire;     // lineaire ou zigzag
    float           angle;           // angle pour le zigzag
    BoundingBox     bb;              // boite collision
    int             level;           // niveau 1 ou 2
} Ennemi;

/* toutes les donnees du joueur */
typedef struct {
    float        x, y;           // position ecran
    float        vx, vy;         // vitesse
    int          largeur;        // taille
    int          hauteur;
    int          vie;            // vie actuelle
    int          vie_max;        // vie max
    EtatJoueur   etat;           // vivant blesse mort
    SDL_Texture *spritesheet;    // image joueur
    SDL_Rect     frames[8];      // frames animation
    int          nb_frames;      // nombre frames
    int          frame_courante; // frame actuelle
    Uint32       temps_anim;     // timer
    int          delai_anim;     // delai frames
    BoundingBox  bb;             // boite collision
    int          score;          // points gagnes
} Joueur;

/* ================================================
   PROTOTYPES - declaration de toutes les fonctions
   ================================================ */

/* TACHE 1 : init et afficher */
void initEnnemi        (Ennemi *e, SDL_Renderer *r,
                        int level, float x, float y);
void afficherEnnemi    (Ennemi *e, SDL_Renderer *r);
void initES            (ES *es, SDL_Renderer *r,
                        float x, float y, const char *img);
void afficherES        (ES *es, SDL_Renderer *r);
void initJoueur        (Joueur *j, SDL_Renderer *r,
                        float x, float y);
void afficherJoueur    (Joueur *j, SDL_Renderer *r,
                        TTF_Font *font);

/* TACHE 2 : deplacement */
void deplacerEnnemi    (Ennemi *e, int lw, int lh);
void deplacerJoueur    (Joueur *j, const Uint8 *clavier,
                        int lw, int lh);

/* TACHE 3 : animation */
void animerEnnemi      (Ennemi *e);
void animerJoueur      (Joueur *j);

/* TACHE 4 : collision */
int  collisionBB       (BoundingBox a, BoundingBox b);
void mettreAJourBB     (Ennemi *e);
void mettreAJourBB_ES  (ES *es);
void mettreAJourBB_J   (Joueur *j);
void gererCollision_JE (Joueur *j, Ennemi *e);
void gererCollision_JES(Joueur *j, ES *es);

/* TACHE 5 : sante */
void mettreAJourEtat   (Ennemi *e);
void blesserEnnemi     (Ennemi *e, int degats);
void afficherSante     (Ennemi *e, SDL_Renderer *r);

/* liberation memoire */
void libererEnnemi     (Ennemi *e);
void libererES         (ES *es);
void libererJoueur     (Joueur *j);

#endif
