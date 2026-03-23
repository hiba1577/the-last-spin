#include "header.h"

/* Initialisation générale */
int initSystem(WindowData *wd)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return 0;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return 0;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;
    if (TTF_Init() == -1) return 0;

    wd->window = SDL_CreateWindow("Sakka hiba",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);

    wd->renderer = SDL_CreateRenderer(wd->window, -1, SDL_RENDERER_ACCELERATED);

    return 1;
}

/* Chargement image + texte */
int loadMedia(WindowData *wd, Media *m)
{
    m->background = IMG_LoadTexture(wd->renderer, "palestine2.jpg");
    if (!m->background) return 0;

    m->bgPos = (SDL_Rect){0,0,800,600};
    m->showText = 0;

    m->font = TTF_OpenFont("arial.ttf", 24);
    if (!m->font) return 0;

    SDL_Color black = {0,0,0,255};

    SDL_Surface *s = TTF_RenderText_Solid(
        m->font,
        "From the river to the sea, Palestine will be free",
        black
    );

    m->text = SDL_CreateTextureFromSurface(wd->renderer, s);
    SDL_FreeSurface(s);

    m->textPos = (SDL_Rect){100,500,600,50};

    return 1;
}

/* Chargement audio */
int loadAudio(Audio *a)
{
    a->shortSound = Mix_LoadWAV("sad.wav");
    a->music = Mix_LoadMUS("palestine.mp3");
    a->musicPlaying = 0;

    if (a->shortSound)
        Mix_PlayChannel(-1, a->shortSound, 0);

    return 1;
}

/* Gestion événements */
void events(WindowData *wd, Media *m, Audio *a, int *run)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) *run = 0;

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE) *run = 0;
            if (e.key.keysym.sym == SDLK_SPACE && a->shortSound)
                Mix_PlayChannel(-1, a->shortSound, 0);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            m->showText = 1;

            if (a->music)
            {
                if (a->musicPlaying)
                {
                    Mix_HaltMusic();
                    a->musicPlaying = 0;
                }
                else
                {
                    Mix_PlayMusic(a->music, -1);
                    a->musicPlaying = 1;
                }
            }
        }

        if (e.type == SDL_MOUSEMOTION && m->showText)
        {
            m->textPos.x = e.motion.x - m->textPos.w/2;
            m->textPos.y = e.motion.y - m->textPos.h/2;
        }
    }
}

/* Affichage */
void render(WindowData *wd, Media *m)
{
    SDL_RenderClear(wd->renderer);
    SDL_RenderCopy(wd->renderer, m->background, NULL, &m->bgPos);

    if (m->showText)
        SDL_RenderCopy(wd->renderer, m->text, NULL, &m->textPos);

    SDL_RenderPresent(wd->renderer);
}

/* Libération mémoire */
void cleanup(WindowData *wd, Media *m, Audio *a)
{
    SDL_DestroyTexture(m->background);
    SDL_DestroyTexture(m->text);
    TTF_CloseFont(m->font);

    if (a->shortSound) Mix_FreeChunk(a->shortSound);
    if (a->music) Mix_FreeMusic(a->music);

    SDL_DestroyRenderer(wd->renderer);
    SDL_DestroyWindow(wd->window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
