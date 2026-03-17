#include "menu.h"
#include <stdio.h>

/* -------------------------------------------------------
   Utilitaires internes
   ------------------------------------------------------- */
static void loadMenuButton(MenuButton *btn, SDL_Renderer *r,
                           const char *path, int x, int y, int w, int h)
{
    btn->texture  = IMG_LoadTexture(r, path);
    if (!btn->texture)
        printf("MANQUE: %s -> %s\n", path, IMG_GetError());
    btn->position = (SDL_Rect){x, y, w, h};
}

static void drawMenuButton(MenuButton *btn, SDL_Renderer *r)
{
    if (btn->texture)
        SDL_RenderCopy(r, btn->texture, NULL, &btn->position);
}

static void freeMenuButton(MenuButton *btn)
{
    if (btn->texture) {
        SDL_DestroyTexture(btn->texture);
        btn->texture = NULL;
    }
}

/* -------------------------------------------------------
   menuButtonClicked
   ------------------------------------------------------- */
int menuButtonClicked(MenuButton *btn, int mx, int my)
{
    return mx >= btn->position.x &&
           mx <= btn->position.x + btn->position.w &&
           my >= btn->position.y &&
           my <= btn->position.y + btn->position.h;
}

/* -------------------------------------------------------
   initMenu
   ------------------------------------------------------- */
void initMenu(Menu *m, SDL_Renderer *renderer)
{
    m->running = 1;
    m->state   = 0;

    initBackground(&m->bg, renderer, "background.png");

    // Écran 1 : mono / multi / retour
    loadMenuButton(&m->mono,   renderer, "mono.png",   150, 250, 200, 60);
    loadMenuButton(&m->multi,  renderer, "multi.png",  450, 250, 200, 60);
    loadMenuButton(&m->retour, renderer, "retour.png", 300, 480, 200, 60);

    // Écran 2 : avatars / inputs / valider
    loadMenuButton(&m->avatar1, renderer, "avatar1.png", 150, 150, 200, 60);
    loadMenuButton(&m->avatar2, renderer, "avatar2.png", 450, 150, 200, 60);
    loadMenuButton(&m->input1,  renderer, "input1.png",  150, 280, 200, 60);
    loadMenuButton(&m->input2,  renderer, "input2.png",  450, 280, 200, 60);
    loadMenuButton(&m->valider, renderer, "valider.png", 300, 390, 200, 60);
}

/* -------------------------------------------------------
   handleMenuInput
   ------------------------------------------------------- */
void handleMenuInput(Menu *m, SDL_Event *event)
{
    if (event->type == SDL_QUIT)
        m->running = 0;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE)
            m->running = 0;
        if (event->key.keysym.sym == SDLK_RETURN && m->state == 1)
            m->running = 0; // → valider via clavier
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mx = event->button.x, my = event->button.y;

        if (m->state == 0) {
            if (menuButtonClicked(&m->mono,  mx, my) ||
                menuButtonClicked(&m->multi, mx, my))
                m->state = 1;
            if (menuButtonClicked(&m->retour, mx, my))
                m->running = 0;
        }
        else if (m->state == 1) {
            if (menuButtonClicked(&m->valider, mx, my))
                m->running = 0; // → valider
            if (menuButtonClicked(&m->retour,  mx, my))
                m->state = 0;
        }
    }
}

/* -------------------------------------------------------
   drawMenu
   ------------------------------------------------------- */
void drawMenu(Menu *m, SDL_Renderer *renderer)
{
    drawBackground(&m->bg, renderer);

    if (m->state == 0) {
        drawMenuButton(&m->mono,   renderer);
        drawMenuButton(&m->multi,  renderer);
        drawMenuButton(&m->retour, renderer);
    }
    else if (m->state == 1) {
        drawMenuButton(&m->avatar1, renderer);
        drawMenuButton(&m->avatar2, renderer);
        drawMenuButton(&m->input1,  renderer);
        drawMenuButton(&m->input2,  renderer);
        drawMenuButton(&m->valider, renderer);
        m->retour.position = (SDL_Rect){50, 480, 200, 60};
        drawMenuButton(&m->retour,  renderer);
    }
}

/* -------------------------------------------------------
   freeMenu
   ------------------------------------------------------- */
void freeMenu(Menu *m)
{
    freeBackground(&m->bg);
    freeMenuButton(&m->mono);
    freeMenuButton(&m->multi);
    freeMenuButton(&m->retour);
    freeMenuButton(&m->avatar1);
    freeMenuButton(&m->avatar2);
    freeMenuButton(&m->input1);
    freeMenuButton(&m->input2);
    freeMenuButton(&m->valider);
}

/* -------------------------------------------------------
   lancerMenuJoueur  ← NOUVELLE FONCTION
   Boucle complète du menu joueur, appelée depuis main.c
   ------------------------------------------------------- */
void lancerMenuJoueur(Menu *m, SDL_Renderer *renderer)
{
    m->running = 1;
    m->state   = 0;
    SDL_Event e;

    while (m->running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            { m->running = 0; break; }
            handleMenuInput(m, &e);
        }
        SDL_RenderClear(renderer);
        drawMenu(m, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
