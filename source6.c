#include "header6.h"
#include <stdio.h>
#include <string.h>

// -------------------------------------------------------
// Utilitaire : dessiner un bouton
// -------------------------------------------------------
void dessiner_bouton(SDL_Renderer* r, Bouton* b, TTF_Font* f,
                     SDL_Color couleur_normal,
                     SDL_Color couleur_survol,
                     SDL_Color couleur_texte) {
    SDL_Color c = b->survole ? couleur_survol : couleur_normal;
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(r, &b->rect);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &b->rect);

    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, b->texte, couleur_texte);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_Rect dst = {
        b->rect.x + (b->rect.w - surf->w) / 2,
        b->rect.y + (b->rect.h - surf->h) / 2,
        surf->w, surf->h
    };
    SDL_FreeSurface(surf);
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

// -------------------------------------------------------
// Utilitaire : afficher texte centré
// -------------------------------------------------------
static void afficher_texte_centre(SDL_Renderer* r, TTF_Font* f,
                                   const char* texte, SDL_Color couleur,
                                   int y, int largeur_ecran) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, texte, couleur);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_Rect dst = {
        (largeur_ecran - surf->w) / 2,
        y, surf->w, surf->h
    };
    SDL_FreeSurface(surf);
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

// -------------------------------------------------------
// INIT
// -------------------------------------------------------
void enigme_init(MenuEnigme* m, SDL_Renderer* renderer) {

    // --- Assets ---
    m->fond = IMG_LoadTexture(renderer, "fond4.png");
    if (!m->fond)
        printf("ERREUR fond4.png : %s\n", IMG_GetError());

    m->musique_suspense = Mix_LoadMUS("music.mp3");
    if (!m->musique_suspense)
        printf("ERREUR music.mp3 : %s\n", Mix_GetError());

    m->son_hover = Mix_LoadWAV("hover.wav");
    if (!m->son_hover)
        printf("ERREUR hover.wav : %s\n", Mix_GetError());

    // --- Boutons écran CHOIX ---
    m->btn_quiz.rect   = (SDL_Rect){250, 280, 200, 60};
    strcpy(m->btn_quiz.texte, "QUIZ");
    m->btn_quiz.survole = 0;

    m->btn_puzzle.rect = (SDL_Rect){550, 280, 200, 60};
    strcpy(m->btn_puzzle.texte, "PUZZLE");
    m->btn_puzzle.survole = 0;

    // --- Boutons réponses QUIZ ---
    m->btn_A.rect = (SDL_Rect){80,  430, 260, 55};
    strcpy(m->btn_A.texte, "A");
    m->btn_A.survole = 0;

    m->btn_B.rect = (SDL_Rect){370, 430, 260, 55};
    strcpy(m->btn_B.texte, "B");
    m->btn_B.survole = 0;

    m->btn_C.rect = (SDL_Rect){660, 430, 260, 55};
    strcpy(m->btn_C.texte, "C");
    m->btn_C.survole = 0;

    // ===== 6 QUESTIONS THÈME INCEPTION =====

    // Question 1
    strcpy(m->questions[0].question,
           "Dans quel niveau de reve se passe l'hotel ?");
    strcpy(m->questions[0].choix[0], "A. Niveau 1");
    strcpy(m->questions[0].choix[1], "B. Niveau 2");
    strcpy(m->questions[0].choix[2], "C. Niveau 3");
    m->questions[0].bonne_reponse = 1;

    // Question 2
    strcpy(m->questions[1].question,
           "Comment appelle-t-on l'objet qui teste la realite ?");
    strcpy(m->questions[1].choix[0], "A. Un miroir");
    strcpy(m->questions[1].choix[1], "B. Un totem");
    strcpy(m->questions[1].choix[2], "C. Une cle");
    m->questions[1].bonne_reponse = 1;

    // Question 3
    strcpy(m->questions[2].question, "Quel est le totem de Cobb ?");
    strcpy(m->questions[2].choix[0], "A. Une bille");
    strcpy(m->questions[2].choix[1], "B. Un de");
    strcpy(m->questions[2].choix[2], "C. Une toupie");
    m->questions[2].bonne_reponse = 2;

    // Question 4
    strcpy(m->questions[3].question,
           "Comment s'appelle le niveau de reve le plus profond ?");
    strcpy(m->questions[3].choix[0], "A. Le Limbe");
    strcpy(m->questions[3].choix[1], "B. Le Vide");
    strcpy(m->questions[3].choix[2], "C. L'Abime");
    m->questions[3].bonne_reponse = 0;

    // Question 5
    strcpy(m->questions[4].question,
           "Qu'est-ce qu'un kick dans le reve ?");
    strcpy(m->questions[4].choix[0], "A. Une attaque ennemie");
    strcpy(m->questions[4].choix[1], "B. Un signal pour se reveiller");
    strcpy(m->questions[4].choix[2], "C. Un portail vers un reve");
    m->questions[4].bonne_reponse = 1;

    // Question 6
    strcpy(m->questions[5].question, "Qui est Mal dans le jeu ?");
    strcpy(m->questions[5].choix[0], "A. La femme de Cobb");
    strcpy(m->questions[5].choix[1], "B. L'ennemie principale");
    strcpy(m->questions[5].choix[2], "C. La guide des reves");
    m->questions[5].bonne_reponse = 0;

    // --- État initial ---
    m->etat              = ENIGME_CHOIX;
    m->question_courante = 0;
    m->reponse_choisie   = -1;
    m->musique_lancee    = 0;
    m->temps_resultat    = 0;
    m->score             = 0;
    strcpy(m->msg_resultat, "");
}

// -------------------------------------------------------
// GESTION ÉVÉNEMENTS
// -------------------------------------------------------
void enigme_gerer_evenements(MenuEnigme* m, SDL_Event* e, int* etat_global) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    int ancien_quiz   = m->btn_quiz.survole;
    int ancien_puzzle = m->btn_puzzle.survole;
    int ancien_A      = m->btn_A.survole;
    int ancien_B      = m->btn_B.survole;
    int ancien_C      = m->btn_C.survole;

    // --- Survol boutons ---
    if (m->etat == ENIGME_CHOIX) {
        m->btn_quiz.survole   = SDL_PointInRect(
            &(SDL_Point){mx, my}, &m->btn_quiz.rect);
        m->btn_puzzle.survole = SDL_PointInRect(
            &(SDL_Point){mx, my}, &m->btn_puzzle.rect);

        if (m->btn_quiz.survole   && !ancien_quiz)
            Mix_PlayChannel(-1, m->son_hover, 0);
        if (m->btn_puzzle.survole && !ancien_puzzle)
            Mix_PlayChannel(-1, m->son_hover, 0);
    }

    if (m->etat == ENIGME_QUIZ) {
        m->btn_A.survole = SDL_PointInRect(
            &(SDL_Point){mx, my}, &m->btn_A.rect);
        m->btn_B.survole = SDL_PointInRect(
            &(SDL_Point){mx, my}, &m->btn_B.rect);
        m->btn_C.survole = SDL_PointInRect(
            &(SDL_Point){mx, my}, &m->btn_C.rect);

        if (m->btn_A.survole && !ancien_A)
            Mix_PlayChannel(-1, m->son_hover, 0);
        if (m->btn_B.survole && !ancien_B)
            Mix_PlayChannel(-1, m->son_hover, 0);
        if (m->btn_C.survole && !ancien_C)
            Mix_PlayChannel(-1, m->son_hover, 0);
    }

    // --- Clavier ---
    if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_ESCAPE)
            *etat_global = -1;
    }

    // --- Clic souris ---
    if (e->type == SDL_MOUSEBUTTONDOWN &&
        e->button.button == SDL_BUTTON_LEFT) {

        // ÉCRAN CHOIX
        if (m->etat == ENIGME_CHOIX) {

            if (m->btn_quiz.survole) {
                m->etat              = ENIGME_QUIZ;
                m->question_courante = 0;
                m->reponse_choisie   = -1;
                m->score             = 0;
                // Lancer musique suspense
                if (!m->musique_lancee && m->musique_suspense) {
                    Mix_PlayMusic(m->musique_suspense, -1);
                    m->musique_lancee = 1;
                }
                // Charger textes boutons Q1
                Question* q = &m->questions[0];
                strcpy(m->btn_A.texte, q->choix[0]);
                strcpy(m->btn_B.texte, q->choix[1]);
                strcpy(m->btn_C.texte, q->choix[2]);
            }

            if (m->btn_puzzle.survole) {
                printf("Puzzle : a implementer !\n");
            }
        }

        // ÉCRAN QUIZ
        else if (m->etat == ENIGME_QUIZ) {
            Question* q = &m->questions[m->question_courante];
            int rep = -1;

            if (m->btn_A.survole) rep = 0;
            if (m->btn_B.survole) rep = 1;
            if (m->btn_C.survole) rep = 2;

            if (rep != -1) {
                m->reponse_choisie = rep;
                if (rep == q->bonne_reponse) {
                    strcpy(m->msg_resultat, "BONNE REPONSE ! +10 pts");
                    m->score += 10;
                } else {
                    strcpy(m->msg_resultat, "MAUVAISE REPONSE !");
                }
                m->etat           = ENIGME_RESULTAT;
                m->temps_resultat = SDL_GetTicks();
            }
        }
    }

    // RÉSULTAT → question suivante après 2 secondes
    if (m->etat == ENIGME_RESULTAT) {
        if (SDL_GetTicks() - m->temps_resultat > 2000) {
            m->question_courante++;
            if (m->question_courante >= 6) {
                // Fin du quiz
                m->etat           = ENIGME_CHOIX;
                m->question_courante = 0;
                Mix_HaltMusic();
                m->musique_lancee = 0;
            } else {
                m->etat            = ENIGME_QUIZ;
                m->reponse_choisie = -1;
                Question* q = &m->questions[m->question_courante];
                strcpy(m->btn_A.texte, q->choix[0]);
                strcpy(m->btn_B.texte, q->choix[1]);
                strcpy(m->btn_C.texte, q->choix[2]);
            }
        }
    }
}

// -------------------------------------------------------
// AFFICHAGE
// -------------------------------------------------------
void enigme_afficher(MenuEnigme* m, SDL_Renderer* renderer, TTF_Font* font) {

    // Fond
    if (m->fond)
        SDL_RenderCopy(renderer, m->fond, NULL, NULL);
    else {
        SDL_SetRenderDrawColor(renderer, 10, 5, 30, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color blanc    = {255, 255, 255, 255};
    SDL_Color jaune    = {255, 220,   0, 255};
    SDL_Color rouge    = {220,  50,  50, 255};
    SDL_Color vert     = { 50, 200,  80, 255};
    SDL_Color cyan     = {  0, 220, 220, 255};
    SDL_Color btn_norm = { 30,  30,  80, 210};
    SDL_Color btn_surv = {100,  60, 200, 255};

    // ===== ÉCRAN CHOIX =====
    if (m->etat == ENIGME_CHOIX) {
        afficher_texte_centre(renderer, font,
            "SOUS-MENU ENIGME", jaune, 80, 1000);
        afficher_texte_centre(renderer, font,
            "Choisissez un mode :", blanc, 200, 1000);

        dessiner_bouton(renderer, &m->btn_quiz,   font,
                        btn_norm, btn_surv, blanc);
        dessiner_bouton(renderer, &m->btn_puzzle, font,
                        btn_norm, btn_surv, blanc);
    }

    // ===== ÉCRAN QUIZ =====
    else if (m->etat == ENIGME_QUIZ) {
        Question* q = &m->questions[m->question_courante];

        // Titre
        afficher_texte_centre(renderer, font, "QUIZ", jaune, 40, 1000);

        // Score
        char score_txt[32];
        sprintf(score_txt, "Score : %d", m->score);
        SDL_Surface* s = TTF_RenderUTF8_Blended(font, score_txt, cyan);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_Rect dst = {850, 40, s->w, s->h};
            SDL_FreeSurface(s);
            SDL_RenderCopy(renderer, t, NULL, &dst);
            SDL_DestroyTexture(t);
        }

        // Numéro question
        char num[32];
        sprintf(num, "Question %d / 6", m->question_courante + 1);
        afficher_texte_centre(renderer, font, num, blanc, 110, 1000);

        // Bande question
        SDL_Rect bande = {50, 180, 900, 80};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &bande);
        SDL_SetRenderDrawColor(renderer, 255, 220, 0, 150);
        SDL_RenderDrawRect(renderer, &bande);

        afficher_texte_centre(renderer, font,
            q->question, blanc, 207, 1000);

        // Boutons A / B / C
        dessiner_bouton(renderer, &m->btn_A, font,
                        btn_norm, btn_surv, blanc);
        dessiner_bouton(renderer, &m->btn_B, font,
                        btn_norm, btn_surv, blanc);
        dessiner_bouton(renderer, &m->btn_C, font,
                        btn_norm, btn_surv, blanc);
    }

    // ===== ÉCRAN RÉSULTAT =====
    else if (m->etat == ENIGME_RESULTAT) {
        Question* q = &m->questions[m->question_courante];

        afficher_texte_centre(renderer, font, "QUIZ", jaune, 40, 1000);

        // Bande question
        SDL_Rect bande = {50, 180, 900, 80};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &bande);
        SDL_SetRenderDrawColor(renderer, 255, 220, 0, 150);
        SDL_RenderDrawRect(renderer, &bande);

        afficher_texte_centre(renderer, font,
            q->question, blanc, 207, 1000);

        // Message bonne/mauvaise réponse
        SDL_Color couleur_msg =
            (m->reponse_choisie == q->bonne_reponse) ? vert : rouge;
        afficher_texte_centre(renderer, font,
            m->msg_resultat, couleur_msg, 310, 1000);

        // Rappel bonne réponse
        char rappel[160];
        sprintf(rappel, "Bonne reponse : %s",
                q->choix[q->bonne_reponse]);
        afficher_texte_centre(renderer, font, rappel, vert, 380, 1000);

        // Score actuel
        char score_txt[32];
        sprintf(score_txt, "Score : %d / 60", m->score);
        afficher_texte_centre(renderer, font,
            score_txt, cyan, 450, 1000);

        // Message fin si dernière question
        if (m->question_courante == 5) {
            afficher_texte_centre(renderer, font,
                "Quiz termine ! Retour au menu...", jaune, 510, 1000);
        }
    }
}


void enigme_lancer(MenuEnigme* m, SDL_Renderer* r,
                   TTF_Font* f, int* quit)
{
    int running = 1;
    SDL_Event e;
    Mix_HaltMusic();

    // Réinitialiser
    m->etat               = ENIGME_CHOIX;
    m->question_courante  = 0;
    m->reponse_choisie    = -1;
    m->score              = 0;

    while (running && !(*quit)) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            { *quit = 1; running = 0; break; }
            enigme_gerer_evenements(m, &e, &running);
        }
        SDL_RenderClear(r);
        enigme_afficher(m, r, f);
        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}
// -------------------------------------------------------
// LIBÉRATION MÉMOIRE
// -------------------------------------------------------
void enigme_liberer(MenuEnigme* m) {
    if (m->fond)             SDL_DestroyTexture(m->fond);
    if (m->musique_suspense) Mix_FreeMusic(m->musique_suspense);
    if (m->son_hover)        Mix_FreeChunk(m->son_hover);
}
