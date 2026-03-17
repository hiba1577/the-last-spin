/*#include "option.h"
#include <stdio.h>

// Charge une image et la transforme en texture SDL
static SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Surface *s = IMG_Load(file);
    if(!s)
    {
        printf("Erreur chargement %s\n", file);
        return NULL;
    }
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

// Vérifie si la souris est sur un bouton
static int isMouseOver(SDL_Rect r)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}

// Initialisation du menu option
int initOption(OptionMenu *opt, SDL_Renderer *renderer)
{
    // Chargement images
    opt->background = loadTexture("fond.png", renderer);
    opt->aug1  = loadTexture("augmenter1.png", renderer);
    opt->aug2  = loadTexture("augmenter2.png", renderer);
    opt->dim1  = loadTexture("diminuer1.png", renderer);
    opt->dim2  = loadTexture("diminuer2.png", renderer);
    opt->full1 = loadTexture("pecran1.png", renderer);
    opt->full2 = loadTexture("pecran2.png", renderer);
    opt->norm1 = loadTexture("normal1.png", renderer);
    opt->norm2 = loadTexture("normal2.png", renderer);
    opt->ret1  = loadTexture("retour1.png", renderer);
    opt->ret2  = loadTexture("retour2.png", renderer);

    // --- MODIFICATION DES POSITIONS (GRILLE) ---
    // Format : {x, y, largeur, hauteur}
    
    // Ligne 1 : Volume
    opt->rDim  = (SDL_Rect){350, 100, 180, 60}; // Bouton "Diminuer" (Gauche)
    opt->rAug  = (SDL_Rect){580, 100, 180, 60}; // Bouton "Augmenter" (Droite)

    // Ligne 2 : Mode d'affichage
    opt->rNorm = (SDL_Rect){350, 250, 180, 60}; // Bouton "Normal" (Gauche)
    opt->rFull = (SDL_Rect){580, 250, 180, 60}; // Bouton "Plein écran" (Droite)

    // Bouton Retour (Bas Droite)
    opt->rRet  = (SDL_Rect){650, 480, 130, 50}; 
    // ------------------------------------------

    opt->volume = MIX_MAX_VOLUME / 2;
    Mix_VolumeMusic(opt->volume);

    opt->music = Mix_LoadMUS("music.mp3");
    if(opt->music) Mix_PlayMusic(opt->music, -1);

    opt->hover = Mix_LoadWAV("hover.wav");
    opt->hoverPlayed = 0;

    return 1;
}

// Gestion clavier + souris (inchangé mais inclus pour la cohérence)
void handleOptionEvents(OptionMenu *opt, SDL_Event *e, SDL_Window *window, int *running)
{
    if(e->type == SDL_QUIT) *running = 0;

    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        if(isMouseOver(opt->rAug)) {
            if(opt->volume < MIX_MAX_VOLUME) opt->volume += 8;
            Mix_VolumeMusic(opt->volume);
        }
        if(isMouseOver(opt->rDim)) {
            if(opt->volume > 0) opt->volume -= 8;
            Mix_VolumeMusic(opt->volume);
        }
        if(isMouseOver(opt->rFull)) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        if(isMouseOver(opt->rNorm)) SDL_SetWindowFullscreen(window, 0);
        if(isMouseOver(opt->rRet)) *running = 0;
    }
}

// Rendu (inchangé)
void renderOption(OptionMenu *opt, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, opt->background, NULL, NULL);

    int hAug  = isMouseOver(opt->rAug);
    int hDim  = isMouseOver(opt->rDim);
    int hFull = isMouseOver(opt->rFull);
    int hNorm = isMouseOver(opt->rNorm);
    int hRet  = isMouseOver(opt->rRet);

    if((hAug||hDim||hFull||hNorm||hRet) && !opt->hoverPlayed && opt->hover) {
        Mix_PlayChannel(-1, opt->hover, 0);
        opt->hoverPlayed = 1;
    }
    if(!(hAug||hDim||hFull||hNorm||hRet)) opt->hoverPlayed = 0;

    SDL_RenderCopy(renderer, hAug  ? opt->aug2  : opt->aug1,  NULL, &opt->rAug);
    SDL_RenderCopy(renderer, hDim  ? opt->dim2  : opt->dim1,  NULL, &opt->rDim);
    SDL_RenderCopy(renderer, hFull ? opt->full2 : opt->full1, NULL, &opt->rFull);
    SDL_RenderCopy(renderer, hNorm ? opt->norm2 : opt->norm1, NULL, &opt->rNorm);
    SDL_RenderCopy(renderer, hRet  ? opt->ret2  : opt->ret1,  NULL, &opt->rRet);
}

// Libération (inchangé)
void freeOption(OptionMenu *opt)
{
    SDL_DestroyTexture(opt->background);
    SDL_DestroyTexture(opt->aug1); SDL_DestroyTexture(opt->aug2);
    SDL_DestroyTexture(opt->dim1); SDL_DestroyTexture(opt->dim2);
    SDL_DestroyTexture(opt->full1); SDL_DestroyTexture(opt->full2);
    SDL_DestroyTexture(opt->norm1); SDL_DestroyTexture(opt->norm2);
    SDL_DestroyTexture(opt->ret1); SDL_DestroyTexture(opt->ret2);
    Mix_FreeMusic(opt->music);
    Mix_FreeChunk(opt->hover);
}
*/
#include "option.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


//////////////////////////////////////////////////////////////
// Chargement image -> texture
//////////////////////////////////////////////////////////////
static SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Surface *s = IMG_Load(file);
    if(!s)
    {
        printf("Erreur chargement %s : %s\n", file, IMG_GetError());
        return NULL;
    }

    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

//////////////////////////////////////////////////////////////
// Création texture texte
//////////////////////////////////////////////////////////////
static SDL_Texture* createText(SDL_Renderer *renderer, TTF_Font *font, const char *text)
{
    SDL_Color color = {0, 0, 0}; // Noir

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface)
    {
        printf("Erreur texte : %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

//////////////////////////////////////////////////////////////
// Détection souris sur bouton
//////////////////////////////////////////////////////////////
static int isMouseOver(SDL_Rect r)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    return (x >= r.x && x <= r.x + r.w &&
            y >= r.y && y <= r.y + r.h);
}

//////////////////////////////////////////////////////////////
// Initialisation menu option
//////////////////////////////////////////////////////////////
int initOption(OptionMenu *opt, SDL_Renderer *renderer)
{
    ////////////////////////
    // Chargement images
    ////////////////////////
    opt->background = loadTexture("fond.png", renderer);

    opt->aug1  = loadTexture("augmenter1.png", renderer);
    opt->aug2  = loadTexture("augmenter2.png", renderer);

    opt->dim1  = loadTexture("diminuer1.png", renderer);
    opt->dim2  = loadTexture("diminuer2.png", renderer);

    opt->full1 = loadTexture("pecran1.png", renderer);
    opt->full2 = loadTexture("pecran2.png", renderer);

    opt->norm1 = loadTexture("normal1.png", renderer);
    opt->norm2 = loadTexture("normal2.png", renderer);

    opt->ret1  = loadTexture("retour1.png", renderer);
    opt->ret2  = loadTexture("retour2.png", renderer);

    ////////////////////////
    // Positions boutons
    ////////////////////////

    // Ligne Volume
    opt->rDim  = (SDL_Rect){350, 100, 180, 60};
    opt->rAug  = (SDL_Rect){580, 100, 180, 60};

    // Ligne Mode
    opt->rNorm = (SDL_Rect){350, 250, 180, 60};
    opt->rFull = (SDL_Rect){580, 250, 180, 60};

    // Retour
    opt->rRet  = (SDL_Rect){650, 480, 130, 50};

    ////////////////////////
    // Texte
    ////////////////////////

    opt->font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 28);
    if(!opt->font)
    {
        printf("Erreur police : %s\n", TTF_GetError());
        return 0;
    }

    opt->txtVolume = createText(renderer, opt->font, "Volume");
    opt->txtMode   = createText(renderer, opt->font, "Mode d'affichage");

    // Position texte Volume
    opt->rTxtVolume.x = 150;
    opt->rTxtVolume.y = 110;
    SDL_QueryTexture(opt->txtVolume, NULL, NULL,
                     &opt->rTxtVolume.w, &opt->rTxtVolume.h);

    // Position texte Mode
    opt->rTxtMode.x = 150;
    opt->rTxtMode.y = 260;
    SDL_QueryTexture(opt->txtMode, NULL, NULL,
                     &opt->rTxtMode.w, &opt->rTxtMode.h);

    ////////////////////////
    // Son
    ////////////////////////

    opt->volume = MIX_MAX_VOLUME / 2;
    Mix_VolumeMusic(opt->volume);

    opt->music = Mix_LoadMUS("music.mp3");
    if(opt->music)
        Mix_PlayMusic(opt->music, -1);

    opt->hover = Mix_LoadWAV("hover.wav");
    opt->hoverPlayed = 0;

    return 1;
}

//////////////////////////////////////////////////////////////
// Gestion événements
//////////////////////////////////////////////////////////////
void handleOptionEvents(OptionMenu *opt, SDL_Event *e,
                        SDL_Window *window, int *running)
{
    if(e->type == SDL_QUIT)
        *running = 0;

    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        if(isMouseOver(opt->rAug))
        {
            if(opt->volume < MIX_MAX_VOLUME)
                opt->volume += 8;

            Mix_VolumeMusic(opt->volume);
        }

        if(isMouseOver(opt->rDim))
        {
            if(opt->volume > 0)
                opt->volume -= 8;

            Mix_VolumeMusic(opt->volume);
        }

        if(isMouseOver(opt->rFull))
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

        if(isMouseOver(opt->rNorm))
            SDL_SetWindowFullscreen(window, 0);

        if(isMouseOver(opt->rRet))
            *running = 0;
    }
}

//////////////////////////////////////////////////////////////
// Rendu
//////////////////////////////////////////////////////////////
void renderOption(OptionMenu *opt, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, opt->background, NULL, NULL);

    // Affichage texte
    SDL_RenderCopy(renderer, opt->txtVolume, NULL, &opt->rTxtVolume);
    SDL_RenderCopy(renderer, opt->txtMode, NULL, &opt->rTxtMode);

    int hAug  = isMouseOver(opt->rAug);
    int hDim  = isMouseOver(opt->rDim);
    int hFull = isMouseOver(opt->rFull);
    int hNorm = isMouseOver(opt->rNorm);
    int hRet  = isMouseOver(opt->rRet);

    if((hAug||hDim||hFull||hNorm||hRet) &&
       !opt->hoverPlayed && opt->hover)
    {
        Mix_PlayChannel(-1, opt->hover, 0);
        opt->hoverPlayed = 1;
    }

    if(!(hAug||hDim||hFull||hNorm||hRet))
        opt->hoverPlayed = 0;

    SDL_RenderCopy(renderer, hAug  ? opt->aug2  : opt->aug1,  NULL, &opt->rAug);
    SDL_RenderCopy(renderer, hDim  ? opt->dim2  : opt->dim1,  NULL, &opt->rDim);
    SDL_RenderCopy(renderer, hFull ? opt->full2 : opt->full1, NULL, &opt->rFull);
    SDL_RenderCopy(renderer, hNorm ? opt->norm2 : opt->norm1, NULL, &opt->rNorm);
    SDL_RenderCopy(renderer, hRet  ? opt->ret2  : opt->ret1,  NULL, &opt->rRet);
}

//////////////////////////////////////////////////////////////
// Libération mémoire
//////////////////////////////////////////////////////////////
void freeOption(OptionMenu *opt)
{
    SDL_DestroyTexture(opt->background);

    SDL_DestroyTexture(opt->aug1);
    SDL_DestroyTexture(opt->aug2);
    SDL_DestroyTexture(opt->dim1);
    SDL_DestroyTexture(opt->dim2);
    SDL_DestroyTexture(opt->full1);
    SDL_DestroyTexture(opt->full2);
    SDL_DestroyTexture(opt->norm1);
    SDL_DestroyTexture(opt->norm2);
    SDL_DestroyTexture(opt->ret1);
    SDL_DestroyTexture(opt->ret2);

    SDL_DestroyTexture(opt->txtVolume);
    SDL_DestroyTexture(opt->txtMode);

    TTF_CloseFont(opt->font);

    Mix_FreeMusic(opt->music);
    Mix_FreeChunk(opt->hover);
}
