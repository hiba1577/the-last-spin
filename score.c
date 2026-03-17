#include "score.h"

/* ============================================================
   Utilitaires scores
   ============================================================ */

void sortScores(Player tab[], int n)
{
    Player tmp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (tab[j].score > tab[i].score)
            {
                tmp    = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp;
            }
}

void saveScore(Player p)
{
    FILE *f = fopen("scores.txt", "a");
    if (f) { fprintf(f, "%s %d\n", p.name, p.score); fclose(f); }
    else   printf("Impossible d'ouvrir scores.txt\n");
}

void loadTop3(Player top3[])
{
    for (int i = 0; i < 3; i++) { strcpy(top3[i].name, "---"); top3[i].score = 0; }

    FILE *f = fopen("scores.txt", "r");
    if (!f) return;

    Player all[MAX_SCORES];
    int n = 0;
    while (n < MAX_SCORES && fscanf(f, "%19s %d", all[n].name, &all[n].score) == 2)
        n++;
    fclose(f);

    sortScores(all, n);
    for (int i = 0; i < 3 && i < n; i++) top3[i] = all[i];
}

/* ============================================================
   Rendu texte interne
   ============================================================ */

static void drawText(SDL_Renderer *r, TTF_Font *font,
                     const char *text, int x, int y, SDL_Color c)
{
    SDL_Surface *s = TTF_RenderUTF8_Blended(font, text, c);
    if (!s) return;
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_Rect rect  = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &rect);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

/* ============================================================
   Init
   ============================================================ */

int initScoreMenu(ScoreMenu *menu, SDL_Renderer *renderer)
{
    menu->background  = IMG_LoadTexture(renderer, "background3.png");
    menu->btn_valider = IMG_LoadTexture(renderer, "valider.png");
    menu->btn_retour  = IMG_LoadTexture(renderer, "retour.png");
    menu->btn_quit    = IMG_LoadTexture(renderer, "quit.png");
    menu->star1       = IMG_LoadTexture(renderer, "star1.png");
    menu->star2       = IMG_LoadTexture(renderer, "star2.png");
    menu->star3       = IMG_LoadTexture(renderer, "star3.png");
    menu->font        = TTF_OpenFont(FONT_PATH, 26);
    menu->victory     = Mix_LoadMUS("victory.mp3");
    menu->hover_sound = Mix_LoadWAV("hover.wav");
    menu->current_score = 0;

    /* debug */
    if (!menu->background)  printf("MANQUE: background3.png -> %s\n", IMG_GetError());
    if (!menu->btn_valider) printf("MANQUE: valider.png     -> %s\n", IMG_GetError());
    if (!menu->btn_retour)  printf("MANQUE: retour.png      -> %s\n", IMG_GetError());
    if (!menu->btn_quit)    printf("MANQUE: quit.png        -> %s\n", IMG_GetError());
    if (!menu->star1)       printf("MANQUE: star1.png       -> %s\n", IMG_GetError());
    if (!menu->star2)       printf("MANQUE: star2.png       -> %s\n", IMG_GetError());
    if (!menu->star3)       printf("MANQUE: star3.png       -> %s\n", IMG_GetError());
    if (!menu->font)        printf("MANQUE: police          -> %s\n", TTF_GetError());
    if (!menu->victory)     printf("MANQUE: victory.mp3     -> %s\n", Mix_GetError());
    if (!menu->hover_sound) printf("MANQUE: hover.wav       -> %s\n", Mix_GetError());

    if (!menu->btn_valider || !menu->btn_retour ||
        !menu->btn_quit    || !menu->font)
    {
        printf("Erreur : ressources indispensables manquantes.\n");
        return 0;
    }

    /* --- positions boutons (fenetre 800x600) --- */
    menu->rect_valider = (SDL_Rect){325, 370, 150, 55};
    menu->rect_retour  = (SDL_Rect){130, 510, 150, 55};
    menu->rect_quit    = (SDL_Rect){520, 510, 150, 55};

    /* --- positions étoiles top3 --- */
    menu->rect_star[0] = (SDL_Rect){160, 430, 50, 50};
    menu->rect_star[1] = (SDL_Rect){160, 490, 50, 50};
    menu->rect_star[2] = (SDL_Rect){160, 550, 50, 50};

    return 1;
}

/* ============================================================
   Ecran 1 : saisie du nom
   ============================================================ */

static void renderSaisie(ScoreMenu *menu, SDL_Renderer *renderer,
                         const char *input, SDL_Rect *hovered)
{
    SDL_RenderClear(renderer);
    if (menu->background)
        SDL_RenderCopy(renderer, menu->background, NULL, NULL);

    SDL_Color blanc       = {255, 255, 255, 255};
    SDL_Color jaune       = {255, 220,  50, 255};
    SDL_Color gris        = {180, 180, 180, 255};
    SDL_Color inputColor  = {255, 255, 100, 255};
    SDL_Color placeholder = {150, 150, 150, 255};

    /* titre */
    drawText(renderer, menu->font, "Entrez votre nom :", 240, 180, blanc);

    /* case de saisie */
    SDL_Rect inputBox = {200, 230, 400, 55};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 180);
    SDL_RenderFillRect(renderer, &inputBox);
    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    SDL_RenderDrawRect(renderer, &inputBox);
    SDL_Rect ib2 = {inputBox.x+1, inputBox.y+1, inputBox.w-2, inputBox.h-2};
    SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255);
    SDL_RenderDrawRect(renderer, &ib2);

    /* texte / placeholder / curseur */
    int tw, th;
    Uint32 ticks = SDL_GetTicks();
    char display[MAX_NAME + 2];
    if ((ticks / 500) % 2 == 0)
        snprintf(display, sizeof(display), "%s|", input);
    else
        snprintf(display, sizeof(display), "%s ", input);

    TTF_SizeUTF8(menu->font, display, &tw, &th);
    int tx = inputBox.x + 12;
    int ty = inputBox.y + (inputBox.h - th) / 2;

    if (strlen(input) == 0)
        drawText(renderer, menu->font, "Votre nom...", tx, ty, placeholder);
    else
        drawText(renderer, menu->font, display, tx, ty, inputColor);

    /* score */
    char stxt[64];
    snprintf(stxt, sizeof(stxt), "Score : %d", menu->current_score);
    drawText(renderer, menu->font, stxt, 330, 310, jaune);

    /* bouton VALIDER */
    SDL_SetTextureAlphaMod(menu->btn_valider,
                           hovered == &menu->rect_valider ? 160 : 255);
    SDL_RenderCopy(renderer, menu->btn_valider, NULL, &menu->rect_valider);

    /* boutons RETOUR / QUITTER */
    SDL_SetTextureAlphaMod(menu->btn_retour,
                           hovered == &menu->rect_retour ? 160 : 255);
    SDL_RenderCopy(renderer, menu->btn_retour, NULL, &menu->rect_retour);

    SDL_SetTextureAlphaMod(menu->btn_quit,
                           hovered == &menu->rect_quit ? 160 : 255);
    SDL_RenderCopy(renderer, menu->btn_quit, NULL, &menu->rect_quit);

    drawText(renderer, menu->font, "Appuyez sur E : sous-menu enigme", 180, 570, gris);
    SDL_RenderPresent(renderer);
}

/* ============================================================
   Ecran 2 : affichage top 3 (ecran separé)
   ============================================================ */

static void renderTop3(ScoreMenu *menu, SDL_Renderer *renderer, SDL_Rect *hovered)
{
    SDL_RenderClear(renderer);

    /* fond : on réutilise background3, ou couleur sombre si absent */
    if (menu->background)
        SDL_RenderCopy(renderer, menu->background, NULL, NULL);
    else
    {
        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 220,  50, 255};
    SDL_Color or_   = {255, 180,   0, 255};
    SDL_Color argent= {200, 200, 200, 255};
    SDL_Color bronze= {180, 100,  40, 255};
    SDL_Color gris  = {180, 180, 180, 255};

    /* panneau semi-transparent centré */
    SDL_Rect panel = {150, 80, 500, 400};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_RenderFillRect(renderer, &panel);
    SDL_SetRenderDrawColor(renderer, 255, 220, 50, 200);
    SDL_RenderDrawRect(renderer, &panel);

    /* titre */
    drawText(renderer, menu->font, "Meilleurs Scores", 250, 100, jaune);

    /* séparateur */
    SDL_SetRenderDrawColor(renderer, 255, 220, 50, 255);
    SDL_RenderDrawLine(renderer, 170, 145, 630, 145);

    SDL_Texture *stars[3] = {menu->star1, menu->star2, menu->star3};
    SDL_Color    colors[3]= {or_, argent, bronze};
    int          ypos[3]  = {170, 270, 370};
    int          starSize = 70;

    for (int i = 0; i < 3; i++)
    {
        /* étoile grande */
        SDL_Rect sr = {175, ypos[i], starSize, starSize};
        if (stars[i]) SDL_RenderCopy(renderer, stars[i], NULL, &sr);

        /* rang */
        char rang[4];
        snprintf(rang, sizeof(rang), "%d.", i + 1);
        drawText(renderer, menu->font, rang, 255, ypos[i] + 15, colors[i]);

        /* nom */
        drawText(renderer, menu->font, menu->top3[i].name, 295, ypos[i] + 10, blanc);

        /* score */
        char pts[32];
        snprintf(pts, sizeof(pts), "%d pts", menu->top3[i].score);
        drawText(renderer, menu->font, pts, 490, ypos[i] + 10, colors[i]);

        /* ligne séparatrice entre entrées */
        if (i < 2)
        {
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
            SDL_RenderDrawLine(renderer, 170, ypos[i] + 80, 630, ypos[i] + 80);
        }
    }

    /* boutons RETOUR / QUITTER */
    SDL_SetTextureAlphaMod(menu->btn_retour,
                           hovered == &menu->rect_retour ? 160 : 255);
    SDL_RenderCopy(renderer, menu->btn_retour, NULL, &menu->rect_retour);

    SDL_SetTextureAlphaMod(menu->btn_quit,
                           hovered == &menu->rect_quit ? 160 : 255);
    SDL_RenderCopy(renderer, menu->btn_quit, NULL, &menu->rect_quit);

    drawText(renderer, menu->font, "Appuyez sur E : sous-menu enigme", 180, 570, gris);
    SDL_RenderPresent(renderer);
}

/* ============================================================
   Boucle événements
   ============================================================ */

void handleScoreMenu(ScoreMenu *menu, SDL_Renderer *renderer,
                     SDL_Window *window, int *quit_game)
{
    (void)window; /* gardé pour extension future (ex: afficher sous-menu enigme) */

    SDL_Event  e;
    int        running    = 1;
    int        showScores = 0;
    SDL_Rect  *hovered    = NULL;
    SDL_Rect  *prev_hov   = NULL;
    char       input[MAX_NAME] = "";

    /* PAS de musique de fond à l'entrée dans ce menu */
    Mix_HaltMusic();

    SDL_StartTextInput();

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            /* ---- quitter fenetre ---- */
            case SDL_QUIT:
                running    = 0;
                *quit_game = 1;
                break;

            /* ---- clavier ---- */
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    /* Quitter le jeu */
                    running    = 0;
                    *quit_game = 1;
                    break;

                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    /* Valider le nom et afficher le top 3 */
                    if (!showScores && strlen(input) > 0)
                    {
                        Player p;
                        strncpy(p.name, input, MAX_NAME - 1);
                        p.name[MAX_NAME - 1] = '\0';
                        p.score = menu->current_score;
                        saveScore(p);
                        loadTop3(menu->top3);
                        showScores = 1;
                        Mix_HaltMusic();
                        if (menu->victory) Mix_PlayMusic(menu->victory, -1);
                    }
                    break;

                case SDLK_BACKSPACE:
                    if (strlen(input) > 0)
                        input[strlen(input) - 1] = '\0';
                    break;

                case SDLK_e:
                    /* Afficher sous-menu énigme (à brancher selon ton projet) */
                    printf("Sous-menu enigme (a implementer)\n");
                    /* lancerEnigme(window, renderer); */
                    break;

                default:
                    break;
                }
                break;

            /* ---- saisie texte ---- */
            case SDL_TEXTINPUT:
                if (!showScores &&
                    strlen(input) + strlen(e.text.text) < (size_t)(MAX_NAME - 1))
                    strcat(input, e.text.text);
                break;

            /* ---- souris : survol ---- */
            case SDL_MOUSEMOTION:
            {
                SDL_Point pt = {e.motion.x, e.motion.y};
                prev_hov = hovered;

                if      (SDL_PointInRect(&pt, &menu->rect_valider)) hovered = &menu->rect_valider;
                else if (SDL_PointInRect(&pt, &menu->rect_retour))  hovered = &menu->rect_retour;
                else if (SDL_PointInRect(&pt, &menu->rect_quit))    hovered = &menu->rect_quit;
                else                                                 hovered = NULL;

                /* son bref uniquement au changement de bouton */
                if (hovered && hovered != prev_hov && menu->hover_sound)
                    Mix_PlayChannel(-1, menu->hover_sound, 0);
                break;
            }

            /* ---- souris : clic ---- */
            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_Point pt = {e.button.x, e.button.y};

                /* VALIDER */
                if (SDL_PointInRect(&pt, &menu->rect_valider) && !showScores)
                {
                    if (strlen(input) > 0)
                    {
                        Player p;
                        strncpy(p.name, input, MAX_NAME - 1);
                        p.name[MAX_NAME - 1] = '\0';
                        p.score = menu->current_score;
                        saveScore(p);
                        loadTop3(menu->top3);
                        showScores = 1;
                        Mix_HaltMusic();
                        if (menu->victory) Mix_PlayMusic(menu->victory, -1);
                    }
                }

                /* RETOUR → menu principal */
                if (SDL_PointInRect(&pt, &menu->rect_retour))
                {
                    Mix_HaltMusic();
                    running = 0;
                }

                /* QUITTER */
                if (SDL_PointInRect(&pt, &menu->rect_quit))
                {
                    Mix_HaltMusic();
                    running    = 0;
                    *quit_game = 1;
                }
                break;
            }

            default:
                break;
            }
        }

        if (!showScores)
            renderSaisie(menu, renderer, input, hovered);
        else
            renderTop3(menu, renderer, hovered);
        SDL_Delay(16);
    }

    SDL_StopTextInput();
}

/* ============================================================
   Libération
   ============================================================ */

void freeScoreMenu(ScoreMenu *menu)
{
    if (menu->background)  SDL_DestroyTexture(menu->background);
    if (menu->btn_valider) SDL_DestroyTexture(menu->btn_valider);
    if (menu->btn_retour)  SDL_DestroyTexture(menu->btn_retour);
    if (menu->btn_quit)    SDL_DestroyTexture(menu->btn_quit);
    if (menu->star1)       SDL_DestroyTexture(menu->star1);
    if (menu->star2)       SDL_DestroyTexture(menu->star2);
    if (menu->star3)       SDL_DestroyTexture(menu->star3);
    if (menu->font)        TTF_CloseFont(menu->font);
    if (menu->victory)     Mix_FreeMusic(menu->victory);
    if (menu->hover_sound) Mix_FreeChunk(menu->hover_sound);
}
