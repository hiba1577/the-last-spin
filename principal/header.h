#ifndef HEADER_H              // Vérifie si HEADER_H n’a pas encore été défini
#define HEADER_H              // Définit HEADER_H pour éviter l’inclusion multiple

#include <SDL2/SDL.h>         // Bibliothèque principale SDL (fenêtre, rendu, événements)
#include <SDL2/SDL_image.h>   // Extension SDL pour charger les images (PNG, JPG, etc.)
#include <SDL2/SDL_mixer.h>   // Extension SDL pour gérer l’audio (musique, effets sonores)
#include <SDL2/SDL_ttf.h>     // Extension SDL pour afficher du texte avec des polices TrueType
#include <stdio.h>            // Bibliothèque standard pour les entrées/sorties (printf, etc.)

// Prototype de la fonction d’initialisation
// Initialise SDL, crée la fenêtre et le renderer
int init(SDL_Window **window, SDL_Renderer **renderer);

// Prototype de la fonction de chargement d’image
// Charge une image depuis un fichier et la convertit en texture
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer);

// Prototype de la fonction de détection souris
// Vérifie si la souris est au-dessus d’un rectangle donné
int isMouseOver(SDL_Rect rect);

// Prototype de la fonction de nettoyage
// Libère toutes les ressources et ferme SDL proprement
void cleanUp(SDL_Window *window, SDL_Renderer *renderer);

#endif   // Fin de la protection contre l’inclusion multiple
