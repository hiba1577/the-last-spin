#include "header.h"
#include "option.h"
#include "score.h" 


// ================= init =================
int init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return 0;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))        return 0;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;
    if (TTF_Init() == -1)                                return 0;

    *window = SDL_CreateWindow("Sommexia",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                800, 600, 0);
    if (!*window)   return 0;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) return 0;

    return 1;
}

// ================= loadTexture =================
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Surface *s = IMG_Load(file);
    if (!s) {
        printf("Image manquante: %s\n", file);
        return NULL;
    }
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

// ================= isMouseOver =================
int isMouseOver(SDL_Rect r)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (x >= r.x && x <= r.x + r.w &&
            y >= r.y && y <= r.y + r.h);
}

// ================= cleanUp =================
void cleanUp(SDL_Window *window, SDL_Renderer *renderer)
{
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// ================= lancerOption =================
void lancerOption(SDL_Window *window, SDL_Renderer *renderer)
{
    OptionMenu opt;
    if (!initOption(&opt, renderer)) return;

    int      running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            { running = 0; break; }
            handleOptionEvents(&opt, &e, window, &running);
        }
        SDL_RenderClear(renderer);
        renderOption(&opt, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    freeOption(&opt);
}

// ================= lancerMenuPrincipal =================
EtatJeu lancerMenuPrincipal(SDL_Window *window, SDL_Renderer *renderer,
                             Mix_Music *music, int *quit)
{
    // Assets
    SDL_Texture *fond = loadTexture("fond.png",    renderer);
    SDL_Texture *b1   = loadTexture("jouer1.png",  renderer);
    SDL_Texture *b1h  = loadTexture("jouer2.png",  renderer);
    SDL_Texture *b2   = loadTexture("option1.png", renderer);
    SDL_Texture *b2h  = loadTexture("option2.png", renderer);
    SDL_Texture *b3   = loadTexture("score1.png",  renderer);
    SDL_Texture *b3h  = loadTexture("score2.png",  renderer);
    SDL_Texture *b4   = loadTexture("quit1.png",   renderer);
    SDL_Texture *b4h  = loadTexture("quit2.png",   renderer);

    Mix_Chunk   *hoverSnd = Mix_LoadWAV("hover.wav");
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 48);

    SDL_Color    blanc    = {255, 255, 255, 255};
    SDL_Surface *surf     = TTF_RenderText_Blended(font, "Sommexia", blanc);
    SDL_Texture *title    = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    SDL_Rect titleRect = {30, 20, 300, 60};

    // Boutons
    SDL_Rect r1 = {300, 170, 220, 60}; // Jouer
    SDL_Rect r2 = {300, 260, 220, 60}; // Options
    SDL_Rect r3 = {300, 350, 220, 60}; // Meilleurs Scores
    SDL_Rect r4 = {300, 440, 220, 60}; // Quitter

    EtatJeu  prochain  = ETAT_MENU_PRINCIPAL;
    int      running   = 1;
    int      hoverPlay = 0;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            { *quit = 1; running = 0; break; }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *quit   = 1;
                        running = 0;
                        break;
                    case SDLK_j:
                        prochain = ETAT_MENU_SAUVEGARDE;
                        running  = 0;
                        break;
                    case SDLK_o:
                        prochain = ETAT_MENU_OPTION;
                        running  = 0;
                        break;
                    case SDLK_m:
                        prochain = ETAT_MENU_SCORE;
                        running  = 0;
                        break;
                    case SDLK_e:
                        prochain = ETAT_MENU_ENIGME;
                        running  = 0;
                        break;
                    default: break;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (isMouseOver(r1)) { prochain = ETAT_MENU_SAUVEGARDE; running = 0; }
                if (isMouseOver(r2)) { prochain = ETAT_MENU_OPTION;     running = 0; }
                if (isMouseOver(r3)) { prochain = ETAT_MENU_SCORE;      running = 0; }
                if (isMouseOver(r4)) { *quit = 1;                       running = 0; }
            }
        }

        // Rendu
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fond, NULL, NULL);

        int h1 = isMouseOver(r1);
        int h2 = isMouseOver(r2);
        int h3 = isMouseOver(r3);
        int h4 = isMouseOver(r4);

        if ((h1||h2||h3||h4) && !hoverPlay && hoverSnd)
        { Mix_PlayChannel(-1, hoverSnd, 0); hoverPlay = 1; }
        if (!(h1||h2||h3||h4)) hoverPlay = 0;

        SDL_RenderCopy(renderer, title,     NULL, &titleRect);
        SDL_RenderCopy(renderer, h1?b1h:b1, NULL, &r1);
        SDL_RenderCopy(renderer, h2?b2h:b2, NULL, &r2);
        SDL_RenderCopy(renderer, h3?b3h:b3, NULL, &r3);
        SDL_RenderCopy(renderer, h4?b4h:b4, NULL, &r4);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage local
    SDL_DestroyTexture(fond);
    SDL_DestroyTexture(b1);  SDL_DestroyTexture(b1h);
    SDL_DestroyTexture(b2);  SDL_DestroyTexture(b2h);
    SDL_DestroyTexture(b3);  SDL_DestroyTexture(b3h);
    SDL_DestroyTexture(b4);  SDL_DestroyTexture(b4h);
    SDL_DestroyTexture(title);
    Mix_FreeChunk(hoverSnd);
    TTF_CloseFont(font);

    return prochain;
}
