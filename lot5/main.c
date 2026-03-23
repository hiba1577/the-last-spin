#include "header.h"

#define FPS 60

int main()
{
    /* ===== INIT SDL ===== */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    /* taille ecran automatique */
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    int LW = mode.w;
    int LH = mode.h;

    SDL_Window *window = SDL_CreateWindow(
        "Enigmes Aleatoires - Inception",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LW, LH,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont(
        "/usr/share/fonts/truetype/dejavu/"
        "DejaVuSans.ttf", 22);

    SDL_Texture *bg =
        IMG_LoadTexture(renderer, "background.png");

    /* ============================================
       TACHE 1 : CREER FICHIER ENIGME.TXT
       ============================================ */
    remplirFichier("enigme.txt");

    /* ============================================
       TACHE 2 : GENERER PREMIERE ENIGME
       ============================================ */
    int derniere = -1;
    int score = 0, vies = 3, level = 1;

    Enigme enigme = genererEnigme(
        "enigme.txt", renderer, font,
        &derniere, score, vies, level);

    int rep_joueur = -1;

    /* ===== BOUCLE PRINCIPALE ===== */
    int       quit  = 0;
    SDL_Event event;
    Uint32    delai = 1000 / FPS;

    while (!quit)
    {
        Uint32 debut = SDL_GetTicks();

        /* evenements */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                quit = 1;

            /* clic souris */
            if (event.type == SDL_MOUSEBUTTONDOWN &&
                enigme.etat == ENIGME_EN_COURS)
            {
                int mx = event.button.x;
                int my = event.button.y;
                SDL_Point pt = {mx, my};

                if (SDL_PointInRect(&pt,
                    &enigme.pos_rep1))
                    rep_joueur = 1;
                else if (SDL_PointInRect(&pt,
                    &enigme.pos_rep2))
                    rep_joueur = 2;
                else if (SDL_PointInRect(&pt,
                    &enigme.pos_rep3))
                    rep_joueur = 3;
            }
        }

        /* ==========================================
           TACHE 3 : RESOUDRE + SCORE/VIES/LEVEL
           ========================================== */
        if (rep_joueur != -1 &&
            enigme.etat == ENIGME_EN_COURS)
        {
            resoudreEnigme(&enigme, rep_joueur);
            score  = enigme.score;
            vies   = enigme.vies;
            level  = enigme.level;
            rep_joueur = -1;
            SDL_Delay(2000);

            if (vies > 0) {
                libererEnigme(&enigme);
                /* TACHE 2 : nouvelle sans repetition */
                enigme = genererEnigme(
                    "enigme.txt", renderer, font,
                    &derniere, score, vies, level);
            }
        }

        /* ==========================================
           TACHE 5 : TIMEOUT
           ========================================== */
        if (enigme.etat == ENIGME_EN_COURS &&
            timerEcoule(&enigme))
        {
            resoudreEnigme(&enigme, -1);
            score = enigme.score;
            vies  = enigme.vies;
            SDL_Delay(2000);

            if (vies > 0) {
                libererEnigme(&enigme);
                enigme = genererEnigme(
                    "enigme.txt", renderer, font,
                    &derniere, score, vies, level);
            }
        }

        /* game over */
        if (vies <= 0) {
            SDL_Delay(3000);
            quit = 1;
        }

        /* ===== AFFICHAGE ===== */
        SDL_RenderClear(renderer);

        if (bg)
            SDL_RenderCopy(renderer, bg, NULL, NULL);
        else {
            SDL_SetRenderDrawColor(renderer,
                                   10, 10, 40, 255);
            SDL_RenderClear(renderer);
        }

        /* TACHE 3 : enigme */
        afficherEnigme(&enigme, renderer);

        /* TACHE 4 : HUD */
        afficherHUD(&enigme, renderer);

        /* TACHE 5 : chrono anime */
        if (enigme.etat == ENIGME_EN_COURS)
            afficherChrono(&enigme, renderer);

        SDL_RenderPresent(renderer);

        Uint32 duree = SDL_GetTicks() - debut;
        if (duree < delai) SDL_Delay(delai - duree);
    }

    /* ===== NETTOYAGE ===== */
    libererEnigme(&enigme);
    if (bg)   SDL_DestroyTexture(bg);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
