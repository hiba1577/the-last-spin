#include "header.h"
#include "option.h"
#include "menu_sauvegarde.h"
#include "score.h"
#include "menu.h"
#include "header6.h"

/* ============================================================
   MAIN
   ============================================================ */
int main()
{
    /* -------------------------------------------------------
       1. Init SDL
       ------------------------------------------------------- */
    SDL_Window   *window   = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init(&window, &renderer)) return 1;

    /* -------------------------------------------------------
       2. Init tous les menus
       ------------------------------------------------------- */
    Menu       menuJoueur; initMenu(&menuJoueur, renderer);
    ScoreMenu  scoreMenu;  int scoreOk = initScoreMenu(&scoreMenu, renderer);
    MenuEnigme menuEnigme; enigme_init(&menuEnigme, renderer);
    TTF_Font  *fontEnigme = TTF_OpenFont(FONT_PATH, 22);

    /* -------------------------------------------------------
       3. Musique principale
       ------------------------------------------------------- */
    Mix_Music *music = Mix_LoadMUS("music.mp3");
    if (music) Mix_PlayMusic(music, -1);

    /* -------------------------------------------------------
       4. Boucle principale
       ------------------------------------------------------- */
    EtatJeu etat = ETAT_MENU_PRINCIPAL;
    int     quit = 0;

    while (!quit)
    {
        switch (etat)
        {
            case ETAT_MENU_PRINCIPAL:
                etat = lancerMenuPrincipal(window, renderer, music, &quit);
                break;

            case ETAT_MENU_OPTION:
                lancerOption(window, renderer);
                etat = ETAT_MENU_PRINCIPAL;
                break;

            case ETAT_MENU_SAUVEGARDE:
                lancerSauvegarde(window, renderer);
                etat = ETAT_MENU_PRINCIPAL;
                break;

            case ETAT_MENU_JOUEUR:
                lancerMenuJoueur(&menuJoueur, renderer);
                etat = ETAT_MENU_SCORE;
                break;

            case ETAT_MENU_SCORE:
                scoreMenu.current_score = 0;
                if (scoreOk)
                    handleScoreMenu(&scoreMenu, renderer, window, &quit);
                etat = ETAT_MENU_PRINCIPAL;
                break;

            case ETAT_MENU_ENIGME:
                enigme_lancer(&menuEnigme, renderer, fontEnigme, &quit);
                etat = ETAT_MENU_PRINCIPAL;
                break;

            case ETAT_QUITTER:
                quit = 1;
                break;

            default:
                etat = ETAT_MENU_PRINCIPAL;
                break;
        }

        // Reprendre musique si elle s'est arrêtée
        if (!quit && !Mix_PlayingMusic() && music)
            Mix_PlayMusic(music, -1);
    }

    /* -------------------------------------------------------
       5. Nettoyage
       ------------------------------------------------------- */
    enigme_liberer(&menuEnigme);
    if (fontEnigme) TTF_CloseFont(fontEnigme);
    if (scoreOk)    freeScoreMenu(&scoreMenu);
    freeMenu(&menuJoueur);
    Mix_FreeMusic(music);
    cleanUp(window, renderer);
    return 0;
}
