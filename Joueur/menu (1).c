#include "menu.h"

void initMenu(Menu *m, SDL_Renderer *renderer)
{
    m->running = 1;
    m->state = 0;

    initBackground(&m->bg, renderer, "background.png");

    // First screen buttons
    initButton(&m->mono, renderer, "mono.png", 400, 350, 250, 80);
    initButton(&m->multi, renderer, "multi.png", 700, 350, 250, 80);
    initButton(&m->retour, renderer, "retour.png", 50, 650, 150, 60);

    // Second screen buttons
    initButton(&m->avatar1, renderer, "avatar1.png", 450, 250, 220, 70);
    initButton(&m->avatar2, renderer, "avatar2.png", 750, 250, 220, 70);
    initButton(&m->input1, renderer, "input1.png", 450, 350, 220, 70);
    initButton(&m->input2, renderer, "input2.png", 750, 350, 220, 70);
    initButton(&m->valider, renderer, "valider.png", 600, 470, 220, 70);
    initButton(&m->retour, renderer, "retour1.png", 50, 650, 150, 60);
}

void handleInput(Menu *m, SDL_Event *event)
{
    if (event->type == SDL_QUIT)
        m->running = 0;

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = event->button.x;
        int my = event->button.y;

        // FIRST SCREEN
        if (m->state == 0)
        {
            if (isInside(&m->mono, mx, my) ||
                isInside(&m->multi, mx, my))
            {
                m->state = 1;   // Switch to second screen
            }
        }
        // SECOND SCREEN
        else if (m->state == 1)
        {
            if (isInside(&m->retour, mx, my))
            {
                m->state = 0;   // Go back
            }

            if (isInside(&m->valider, mx, my))
            {
                printf("Open meilleurs scores\n");
            }
        }
    }

    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_RETURN && m->state == 1)
        {
            printf("Open meilleurs scores\n");
        }
    }
}

void drawMenu(Menu *m, SDL_Renderer *renderer)
{
    drawBackground(&m->bg, renderer);

    if (m->state == 0)
    {
        drawButton(&m->mono, renderer);
        drawButton(&m->multi, renderer);
        drawButton(&m->retour, renderer);
    }
    else if (m->state == 1)
    {
        drawButton(&m->avatar1, renderer);
        drawButton(&m->avatar2, renderer);
        drawButton(&m->input1, renderer);
        drawButton(&m->input2, renderer);
        drawButton(&m->valider, renderer);
        drawButton(&m->retour, renderer);
    }
}

void freeMenu(Menu *m)
{
    freeBackground(&m->bg);

    freeButton(&m->mono);
    freeButton(&m->multi);
    freeButton(&m->avatar1);
    freeButton(&m->avatar2);
    freeButton(&m->input1);
    freeButton(&m->input2);
    freeButton(&m->valider);
    freeButton(&m->retour);
}

