#include "header.h"

#define FPS    60
#define NB_ENM  2
#define NB_ES   3

int main()
{
    /* ===== INIT SDL ===== */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    /* ===== RECUPERER TAILLE ECRAN ===== */
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    int LARGEUR = mode.w;   // largeur de l'ecran
    int HAUTEUR = mode.h;   // hauteur de l'ecran

    /* ===== CREER FENETRE PLEIN ECRAN ===== */
    SDL_Window *window = SDL_CreateWindow(
        "Lot 3 - Ennemi Level1/Level2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR, HAUTEUR,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED);

    /* ===== POLICE ===== */
    TTF_Font *font = TTF_OpenFont(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        20);

    /* ===== FOND ===== */
    SDL_Texture *bg =
        IMG_LoadTexture(renderer, "background.png");

    /* ===== TACHE 1 : INIT ===== */
    Joueur joueur;
    initJoueur(&joueur, renderer,
               LARGEUR * 0.1f,   // 10% depuis gauche
               HAUTEUR * 0.5f);  // milieu vertical

    Ennemi ennemis[NB_ENM];
    initEnnemi(&ennemis[0], renderer, 1,
               LARGEUR * 0.5f,   // milieu horizontal
               HAUTEUR * 0.3f);  // haut
    initEnnemi(&ennemis[1], renderer, 2,
               LARGEUR * 0.7f,   // 70% horizontal
               HAUTEUR * 0.6f);  // bas

    ES es[NB_ES];
    initES(&es[0], renderer,
           LARGEUR * 0.3f, HAUTEUR * 0.7f, "es1.png");
    initES(&es[1], renderer,
           LARGEUR * 0.5f, HAUTEUR * 0.4f, "es2.png");
    initES(&es[2], renderer,
           LARGEUR * 0.8f, HAUTEUR * 0.7f, "es3.png");

    /* ===== BOUCLE PRINCIPALE ===== */
    int       quit  = 0;
    SDL_Event event;
    Uint32    delai = 1000 / FPS;

    while (!quit)
    {
        Uint32 debut = SDL_GetTicks();

        /* --- evenements --- */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                quit = 1;
        }

        /* ===== TACHE 2 : DEPLACEMENT ===== */
        const Uint8 *clavier = SDL_GetKeyboardState(NULL);
        deplacerJoueur(&joueur, clavier, LARGEUR, HAUTEUR);
        for (int i = 0; i < NB_ENM; i++)
            deplacerEnnemi(&ennemis[i], LARGEUR, HAUTEUR);

        /* ===== TACHE 3 : ANIMATION ===== */
        animerJoueur(&joueur);
        for (int i = 0; i < NB_ENM; i++)
            animerEnnemi(&ennemis[i]);

        /* ===== TACHE 4 : COLLISION ===== */
        for (int i = 0; i < NB_ENM; i++)
            gererCollision_JE(&joueur, &ennemis[i]);
        for (int i = 0; i < NB_ES; i++)
            gererCollision_JES(&joueur, &es[i]);

        /* ===== TACHE 5 : SANTE ===== */
        for (int i = 0; i < NB_ENM; i++)
            mettreAJourEtat(&ennemis[i]);

        if (joueur.etat == JOUEUR_MORT) {
            printf("GAME OVER ! Score: %d\n", joueur.score);
            quit = 1;
        }

        /* ===== AFFICHAGE ===== */
        SDL_RenderClear(renderer);

        if (bg)
            SDL_RenderCopy(renderer, bg, NULL, NULL);
        else {
            SDL_SetRenderDrawColor(renderer, 10, 10, 40, 255);
            SDL_RenderClear(renderer);
        }

        for (int i = 0; i < NB_ES; i++)
            afficherES(&es[i], renderer);
        for (int i = 0; i < NB_ENM; i++)
            afficherEnnemi(&ennemis[i], renderer);
        for (int i = 0; i < NB_ENM; i++)
            afficherSante(&ennemis[i], renderer);

        afficherJoueur(&joueur, renderer, font);

        SDL_RenderPresent(renderer);

        Uint32 duree = SDL_GetTicks() - debut;
        if (duree < delai) SDL_Delay(delai - duree);
    }

    /* ===== NETTOYAGE ===== */
    libererJoueur(&joueur);
    for (int i = 0; i < NB_ENM; i++)
        libererEnnemi(&ennemis[i]);
    for (int i = 0; i < NB_ES; i++)
        libererES(&es[i]);
    if (bg)   SDL_DestroyTexture(bg);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
