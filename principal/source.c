#include "header.h"   // Inclusion du fichier header contenant les bibliothèques et prototypes

// Fonction d'initialisation de SDL et de ses modules
int init(SDL_Window **window, SDL_Renderer **renderer)
{
    // Initialisation du module vidéo et audio de SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return 0;

    // Initialisation du support PNG avec SDL_image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return 0;

    // Initialisation du système audio SDL_mixer
    // 44100 Hz = fréquence, format par défaut, 2 canaux (stéréo), buffer 2048
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;

    // Initialisation du module SDL_ttf pour le texte
    if(TTF_Init() == -1) return 0;

    // Création de la fenêtre principale
    *window = SDL_CreateWindow("Menu",              // Titre de la fenêtre
                                SDL_WINDOWPOS_CENTERED, // Position X centrée
                                SDL_WINDOWPOS_CENTERED, // Position Y centrée
                                800, 600,               // Largeur 800px, Hauteur 600px
                                0);                     // Aucun flag spécial
    if(!*window) return 0;   // Vérifie si la fenêtre a été créée correctement

    // Création du renderer (moteur de rendu graphique)
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    // -1 = pilote par défaut, SDL_RENDERER_ACCELERATED = utilise la carte graphique
    if(!*renderer) return 0;  // Vérifie si le renderer est valide

    return 1;   // Retourne 1 si tout s’est bien passé
}

// Fonction pour charger une image et la convertir en texture
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Surface *s = IMG_Load(file);   // Charge l'image depuis le fichier vers une surface
    if(!s)                              // Si l'image ne se charge pas
    {
        printf("Image manquante: %s\n", file);  // Affiche un message d’erreur
        return NULL;                            // Retourne NULL
    }

    // Conversion de la surface en texture adaptée au renderer
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);

    SDL_FreeSurface(s);   // Libère la mémoire de la surface (plus nécessaire)

    return t;   // Retourne la texture créée
}

// Fonction qui vérifie si la souris est au-dessus d’un rectangle
int isMouseOver(SDL_Rect r)
{
    int x,y;   // Variables pour stocker la position de la souris

    SDL_GetMouseState(&x,&y);   // Récupère les coordonnées actuelles de la souris

    // Retourne 1 si la souris est dans le rectangle, sinon 0
    return (x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h);
}

// Fonction de libération des ressources et fermeture propre du programme
void cleanUp(SDL_Window *window, SDL_Renderer *renderer)
{
    Mix_CloseAudio();          // Ferme le système audio

    IMG_Quit();                // Quitte SDL_image

    TTF_Quit();                // Quitte SDL_ttf

    SDL_DestroyRenderer(renderer);  // Détruit le renderer

    SDL_DestroyWindow(window);      // Détruit la fenêtre

    SDL_Quit();               // Ferme complètement SDL
}
