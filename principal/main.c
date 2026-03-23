#include "header.h"   // Inclusion du fichier header (SDL + prototypes)

// Définition d’un type énuméré pour gérer les états du programme
typedef enum {MENU, JOUER, OPTION, SCORE, HISTOIRE} Etat;

// Fonction qui crée une texture texte à partir d’une police
SDL_Texture* createText(SDL_Renderer *r, TTF_Font *font, char *msg)
{
    SDL_Color c = {255,255,255,255};              // Couleur blanche (RGBA)
    SDL_Surface *s = TTF_RenderText_Blended(font,msg,c); // Création surface texte
    SDL_Texture *t = SDL_CreateTextureFromSurface(r,s);  // Conversion surface → texture
    SDL_FreeSurface(s);                          // Libération de la surface
    return t;                                    // Retourne la texture texte
}

int main()
{
    SDL_Window *window=NULL;      // Pointeur vers la fenêtre
    SDL_Renderer *renderer=NULL;  // Pointeur vers le moteur de rendu

    if(!init(&window,&renderer)) return 1;  // Initialisation SDL

    Etat etat = MENU;   // État initial = MENU

    SDL_Texture *fond = loadTexture("fond.png",renderer); // Chargement fond

    // Chargement des textures des boutons (normal + hover)
    SDL_Texture *b1=loadTexture("jouer1.png",renderer);
    SDL_Texture *b1h=loadTexture("jouer2.png",renderer);
    SDL_Texture *b2=loadTexture("option1.png",renderer);
    SDL_Texture *b2h=loadTexture("option2.png",renderer);
    SDL_Texture *b3=loadTexture("score1.png",renderer);
    SDL_Texture *b3h=loadTexture("score2.png",renderer);
    SDL_Texture *b4=loadTexture("histoire1.png",renderer);
    SDL_Texture *b4h=loadTexture("histoire2.png",renderer);
    SDL_Texture *b5=loadTexture("quit1.png",renderer);
    SDL_Texture *b5h=loadTexture("quit2.png",renderer);

    // Chargement musique de fond
    Mix_Music *music = Mix_LoadMUS("music.mp3");
    if(music) Mix_PlayMusic(music,-1); // -1 = boucle infinie

    // Chargement son hover
    Mix_Chunk *hover = Mix_LoadWAV("hover.wav");

    // Chargement police
    TTF_Font *font = TTF_OpenFont("arial.ttf",48);

    // Création texture du titre
    SDL_Texture *title = createText(renderer,font,"Sommexia");
    SDL_Rect titleRect={30,20,300,60}; // Position + taille titre

    // Création textes sous-menus
    SDL_Texture *txtJouer=createText(renderer,font,"SOUS MENU JOUER");
    SDL_Texture *txtOpt=createText(renderer,font,"SOUS MENU OPTION");
    SDL_Texture *txtScore=createText(renderer,font,"MEILLEURS SCORES");
    SDL_Rect txtRect={200,250,400,80}; // Position texte centre écran

    // Définition positions boutons
    SDL_Rect r1={300,140,220,60};
    SDL_Rect r2={300,210,220,60};
    SDL_Rect r3={300,280,220,60};
    SDL_Rect r4={300,350,220,60};
    SDL_Rect r5={300,420,220,60};

    SDL_Event e;            // Structure événement
    int quit=0;             // Variable pour quitter boucle
    int hoverPlay=0;        // Empêche répétition son hover

    while(!quit)            // Boucle principale
    {
        while(SDL_PollEvent(&e)) // Gestion événements
        {
            if(e.type==SDL_QUIT) quit=1; // Fermer fenêtre

            if(e.type==SDL_KEYDOWN)      // Si touche clavier appuyée
            {
                if(e.key.keysym.sym==SDLK_ESCAPE) quit=1; // ESC = quitter
                if(e.key.keysym.sym==SDLK_j) etat=JOUER;  // j = jouer
                if(e.key.keysym.sym==SDLK_o) etat=OPTION; // o = option
                if(e.key.keysym.sym==SDLK_m) etat=SCORE;  // m = score
                if(e.key.keysym.sym==SDLK_b) etat=MENU;   // b = retour menu
            }

            // Gestion clic souris uniquement dans MENU
            if(e.type==SDL_MOUSEBUTTONDOWN && etat==MENU)
            {
                if(isMouseOver(r1)) etat=JOUER;
                if(isMouseOver(r2)) etat=OPTION;
                if(isMouseOver(r3)) etat=SCORE;
                if(isMouseOver(r4)) etat=HISTOIRE;
                if(isMouseOver(r5)) quit=1;
            }
        }

        SDL_RenderClear(renderer);              // Efface écran
        SDL_RenderCopy(renderer,fond,NULL,NULL); // Dessine fond

        if(etat==MENU)
        {
            // Vérifie si souris sur boutons
            int h1=isMouseOver(r1),h2=isMouseOver(r2),
                h3=isMouseOver(r3),h4=isMouseOver(r4),
                h5=isMouseOver(r5);

            // Joue son hover une seule fois
            if((h1||h2||h3||h4||h5)&&!hoverPlay&&hover)
            { 
                Mix_PlayChannel(-1,hover,0);
                hoverPlay=1;
            }

            if(!(h1||h2||h3||h4||h5)) hoverPlay=0;

            SDL_RenderCopy(renderer,title,NULL,&titleRect);

            // Affichage boutons normal/hover
            SDL_RenderCopy(renderer,h1?b1h:b1,NULL,&r1);
            SDL_RenderCopy(renderer,h2?b2h:b2,NULL,&r2);
            SDL_RenderCopy(renderer,h3?b3h:b3,NULL,&r3);
            SDL_RenderCopy(renderer,h4?b4h:b4,NULL,&r4);
            SDL_RenderCopy(renderer,h5?b5h:b5,NULL,&r5);
        }
        else if(etat==JOUER)
            SDL_RenderCopy(renderer,txtJouer,NULL,&txtRect);
        else if(etat==OPTION)
            SDL_RenderCopy(renderer,txtOpt,NULL,&txtRect);
        else if(etat==SCORE)
            SDL_RenderCopy(renderer,txtScore,NULL,&txtRect);
        else if(etat==HISTOIRE)
            SDL_RenderCopy(renderer,txtOpt,NULL,&txtRect);

        SDL_RenderPresent(renderer); // Affiche rendu
        SDL_Delay(16);               // ~60 FPS
    }

    // Libération mémoire textures
    SDL_DestroyTexture(fond);
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(txtJouer);
    SDL_DestroyTexture(txtOpt);
    SDL_DestroyTexture(txtScore);

    Mix_FreeMusic(music);     // Libère musique
    Mix_FreeChunk(hover);     // Libère son
    TTF_CloseFont(font);      // Ferme police

    cleanUp(window,renderer); // Nettoyage SDL
    return 0;                 // Fin programme
}
