#include "header.h"

/* utilitaire : creer texture depuis texte */
static SDL_Texture* creerTex(SDL_Renderer *r,
                              TTF_Font *f,
                              const char *txt,
                              SDL_Color c)
{
    SDL_Surface *s = TTF_RenderUTF8_Blended(f, txt, c);
    if (!s) return NULL;
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

/* ================================================
   TACHE 1 : REMPLISSAGE FICHIER + CHARGEMENT
   ================================================ */

/* creer enigme.txt avec 10 questions */
void remplirFichier(const char *fichier)
{
    FILE *f = fopen(fichier, "w");
    if (!f) {
        printf("ERREUR: impossible de creer %s\n",
               fichier);
        return;
    }

    /* 10 questions theme Inception */
    fprintf(f, "Dans quel niveau de reve se passe "
               "l'hotel?Niveau 1.Niveau 2.Niveau 3.2\n");
    fprintf(f, "Quel est le totem de Cobb?"
               "Une bille.Un de.Une toupie.3\n");
    fprintf(f, "Qu'est-ce qu'un kick dans le reve?"
               "Une attaque.Un signal pour se reveiller."
               "Un portail.2\n");
    fprintf(f, "Comment s'appelle le niveau profond?"
               "Le Limbe.Le Vide.L'Abime.1\n");
    fprintf(f, "Qui est Mal dans le jeu?"
               "La femme de Cobb.L'ennemie."
               "La guide des reves.1\n");
    fprintf(f, "Combien de niveaux de reve y a-t-il?"
               "2.3.4.2\n");
    fprintf(f, "Quelle chanson sert de kick?"
               "Non Non Rien."
               "Non Je Ne Regrette Rien."
               "La Vie En Rose.2\n");
    fprintf(f, "Qui est l'architecte des reves?"
               "Ariadne.Yusuf.Eames.1\n");
    fprintf(f, "Que signifie inception?"
               "Extraire une idee.Planter une idee."
               "Detruire une idee.2\n");
    fprintf(f, "Quel est le nom du client principal?"
               "Saito.Fischer.Arthur.1\n");

    fclose(f);
    printf("Fichier %s cree avec 10 questions!\n",
           fichier);
}

/* compter les lignes du fichier */
int compterLignes(const char *fichier)
{
    FILE *f = fopen(fichier, "r");
    if (!f) return 0;

    int  nb = 0;
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f))
        nb++;

    fclose(f);
    return nb;
}

/* decoder une ligne en structure Enigme */
Enigme chargerEnigme(const char *ligne,
                     SDL_Renderer *r, TTF_Font *f,
                     int score, int vies, int level)
{
    Enigme e;
    memset(&e, 0, sizeof(Enigme));
    e.font        = f;
    e.couleur     = (SDL_Color){255, 255, 255, 255};
    e.etat        = ENIGME_EN_COURS;
    e.score       = score;
    e.vies        = vies;
    e.level       = level;
    e.duree       = DUREE_ENIGME;
    e.angle_anim  = 0.0f;
    e.temps_debut = SDL_GetTicks();

    int i = 0, j = 0;

    /* extraire question jusqu'au '?' */
    while (ligne[i] != '?' && ligne[i] != '\0')
        e.question[j++] = ligne[i++];
    e.question[j] = '\0';
    i++;

    /* extraire reponse 1 jusqu'au '.' */
    j = 0;
    while (ligne[i] != '.' && ligne[i] != '\0')
        e.reponse1[j++] = ligne[i++];
    e.reponse1[j] = '\0';
    i++;

    /* extraire reponse 2 */
    j = 0;
    while (ligne[i] != '.' && ligne[i] != '\0')
        e.reponse2[j++] = ligne[i++];
    e.reponse2[j] = '\0';
    i++;

    /* extraire reponse 3 */
    j = 0;
    while (ligne[i] != '.' && ligne[i] != '\0')
        e.reponse3[j++] = ligne[i++];
    e.reponse3[j] = '\0';
    i++;

    /* extraire bonne reponse */
    e.bonne_reponse = ligne[i] - '0';

    /* creer textures */
    SDL_Color jaune = {255, 220,   0, 255};
    SDL_Color blanc = {255, 255, 255, 255};

    e.tex_question = creerTex(r, f, e.question, jaune);
    e.tex_rep1     = creerTex(r, f, e.reponse1, blanc);
    e.tex_rep2     = creerTex(r, f, e.reponse2, blanc);
    e.tex_rep3     = creerTex(r, f, e.reponse3, blanc);

    /* positions selon taille ecran */
    int lw, lh;
    SDL_GetRendererOutputSize(r, &lw, &lh);

    e.pos_question = (SDL_Rect){lw/10,   lh/4,
                                lw*8/10, lh/10};
    e.pos_rep1     = (SDL_Rect){lw/10,   lh*6/10,
                                lw*2/10, lh/12};
    e.pos_rep2     = (SDL_Rect){lw*4/10, lh*6/10,
                                lw*2/10, lh/12};
    e.pos_rep3     = (SDL_Rect){lw*7/10, lh*6/10,
                                lw*2/10, lh/12};
    return e;
}

/* ================================================
   TACHE 2 : GENERATION ALEATOIRE SANS REPETITION
   ================================================ */
Enigme genererEnigme(const char *fichier,
                     SDL_Renderer *r, TTF_Font *f,
                     int *derniere,
                     int score, int vies, int level)
{
    Enigme e;
    memset(&e, 0, sizeof(Enigme));

    int nb = compterLignes(fichier);
    if (nb <= 0) {
        printf("ERREUR: fichier vide!\n");
        return e;
    }

    srand((unsigned int)time(NULL));

    int num;
    /* tirer different du precedent */
    do {
        num = (rand() % nb) + 1;
    } while (num == *derniere && nb > 1);

    *derniere = num; // sauvegarder

    FILE *fp = fopen(fichier, "r");
    if (!fp) return e;

    char ligne[512];
    int  compteur = 0;

    while (fgets(ligne, sizeof(ligne), fp)) {
        compteur++;
        if (compteur == num) {
            ligne[strcspn(ligne, "\n")] = '\0';
            e = chargerEnigme(ligne, r, f,
                              score, vies, level);
            e.numQuestSelect = num;
            break;
        }
    }
    fclose(fp);
    return e;
}

/* ================================================
   TACHE 3 : RESOLUTION + SCORE/VIES/LEVEL
   ================================================ */
void resoudreEnigme(Enigme *e, int rep_joueur)
{
    if (e->etat != ENIGME_EN_COURS) return;

    if (rep_joueur == e->bonne_reponse)
    {
        e->etat  = ENIGME_SUCCES;
        e->score += 10;
        printf("Bonne reponse! Score: %d\n", e->score);

        /* level up tous les 50 points */
        if (e->score > 0 && e->score % 50 == 0) {
            e->level++;
            printf("Level up! Level: %d\n", e->level);
        }
    }
    else
    {
        e->etat = ENIGME_ECHEC;
        e->vies--;
        printf("Mauvaise! Vies: %d\n", e->vies);

        if (e->vies <= 0) {
            e->vies = 0;
            printf("GAME OVER! Score: %d\n", e->score);
        }
    }
}

/* afficher enigme a l'ecran */
void afficherEnigme(Enigme *e, SDL_Renderer *r)
{
    int lw, lh;
    SDL_GetRendererOutputSize(r, &lw, &lh);

    /* fond noir semi-transparent */
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 200);
    SDL_Rect fond = {lw/10, lh/5,
                     lw*8/10, lh*6/10};
    SDL_RenderFillRect(r, &fond);

    /* bordure jaune */
    SDL_SetRenderDrawColor(r, 255, 220, 0, 255);
    SDL_RenderDrawRect(r, &fond);

    /* afficher question */
    if (e->tex_question) {
        int w, h;
        SDL_QueryTexture(e->tex_question,
                         NULL, NULL, &w, &h);
        SDL_Rect dst = {
            e->pos_question.x +
            (e->pos_question.w - w) / 2,
            e->pos_question.y, w, h};
        SDL_RenderCopy(r, e->tex_question,
                       NULL, &dst);
    }

    /* 3 boutons reponses */
    SDL_Color couleurs[3] = {
        {30,  30, 150, 220},
        {30, 120,  30, 220},
        {150, 30,  30, 220}
    };
    SDL_Texture *reps[3] = {
        e->tex_rep1, e->tex_rep2, e->tex_rep3
    };
    SDL_Rect pos[3] = {
        e->pos_rep1, e->pos_rep2, e->pos_rep3
    };

    for (int i = 0; i < 3; i++) {
        SDL_SetRenderDrawColor(r,
            couleurs[i].r, couleurs[i].g,
            couleurs[i].b, couleurs[i].a);
        SDL_RenderFillRect(r, &pos[i]);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawRect(r, &pos[i]);

        if (reps[i]) {
            int w, h;
            SDL_QueryTexture(reps[i],
                             NULL, NULL, &w, &h);
            SDL_Rect dst = {
                pos[i].x + (pos[i].w - w) / 2,
                pos[i].y + (pos[i].h - h) / 2,
                w, h};
            SDL_RenderCopy(r, reps[i], NULL, &dst);
        }
    }

    /* message resultat */
    if (e->etat == ENIGME_SUCCES) {
        SDL_Color vert = {50, 220, 80, 255};
        SDL_Surface *s = TTF_RenderUTF8_Blended(
            e->font,
            "BONNE REPONSE ! +10 pts", vert);
        if (s) {
            SDL_Texture *t =
                SDL_CreateTextureFromSurface(r, s);
            SDL_Rect dst = {lw/2 - s->w/2,
                            lh*75/100, s->w, s->h};
            SDL_RenderCopy(r, t, NULL, &dst);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
    } else if (e->etat == ENIGME_ECHEC) {
        SDL_Color rouge = {220, 50, 50, 255};
        SDL_Surface *s = TTF_RenderUTF8_Blended(
            e->font,
            "MAUVAISE REPONSE ! -1 vie", rouge);
        if (s) {
            SDL_Texture *t =
                SDL_CreateTextureFromSurface(r, s);
            SDL_Rect dst = {lw/2 - s->w/2,
                            lh*75/100, s->w, s->h};
            SDL_RenderCopy(r, t, NULL, &dst);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
    }
}

/* ================================================
   TACHE 4 : HUD score/vies/level
   ================================================ */
void afficherHUD(Enigme *e, SDL_Renderer *r)
{
    if (!e->font) return;

    int lw, lh;
    SDL_GetRendererOutputSize(r, &lw, &lh);

    SDL_Color jaune = {255, 220,   0, 255};
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color rouge = {220,  50,  50, 255};

    /* score haut gauche */
    char txt[32];
    sprintf(txt, "Score: %d", e->score);
    SDL_Surface *s = TTF_RenderUTF8_Blended(
        e->font, txt, jaune);
    if (s) {
        SDL_Texture *t =
            SDL_CreateTextureFromSurface(r, s);
        SDL_Rect dst = {20, 10, s->w, s->h};
        SDL_RenderCopy(r, t, NULL, &dst);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    /* level haut droite */
    sprintf(txt, "Level: %d", e->level);
    s = TTF_RenderUTF8_Blended(e->font, txt, blanc);
    if (s) {
        SDL_Texture *t =
            SDL_CreateTextureFromSurface(r, s);
        SDL_Rect dst = {lw-s->w-20, 10, s->w, s->h};
        SDL_RenderCopy(r, t, NULL, &dst);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    /* vies = carres colores */
    for (int i = 0; i < 3; i++) {
        SDL_Rect c = {20 + i*30, 40, 20, 20};
        if (i < e->vies)
            SDL_SetRenderDrawColor(r,
                220, 50, 50, 255);  // rouge = vie
        else
            SDL_SetRenderDrawColor(r,
                80, 80, 80, 255);   // gris = perdue
        SDL_RenderFillRect(r, &c);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawRect(r, &c);
    }

    /* game over */
    if (e->vies <= 0) {
        s = TTF_RenderUTF8_Blended(
            e->font, "GAME OVER !", rouge);
        if (s) {
            SDL_Texture *t =
                SDL_CreateTextureFromSurface(r, s);
            SDL_Rect dst = {lw/2-s->w/2,
                            lh/2-s->h/2,
                            s->w, s->h};
            SDL_RenderCopy(r, t, NULL, &dst);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
    }
}

/* ================================================
   TACHE 5 : CHRONOMETRE ANIME sans chiffres
   ================================================ */

/* verifier si temps ecoule */
int timerEcoule(Enigme *e)
{
    Uint32 ecoule =
        (SDL_GetTicks() - e->temps_debut) / 1000;
    return (int)ecoule >= e->duree;
}

/* dessiner cercle anime */
void afficherChrono(Enigme *e, SDL_Renderer *r)
{
    int lw, lh;
    SDL_GetRendererOutputSize(r, &lw, &lh);

    /* temps restant */
    Uint32 ecoule =
        (SDL_GetTicks() - e->temps_debut) / 1000;
    int restant = e->duree - (int)ecoule;
    if (restant < 0) restant = 0;

    /* pourcentage restant */
    float pct = (float)restant / (float)e->duree;

    /* rotation animation */
    e->angle_anim += 0.03f;

    /* position centre en haut */
    int cx    = lw / 2;
    int cy    = 70;
    int rayon = 30;

    /* couleur selon temps */
    Uint8 rouge_c, vert_c;
    if (pct > 0.5f) {
        rouge_c = 50;  vert_c = 200; // vert
    } else if (pct > 0.25f) {
        rouge_c = 220; vert_c = 140; // orange
    } else {
        rouge_c = 220; vert_c = 50;  // rouge
    }

    /* cercle fond gris */
    SDL_SetRenderDrawColor(r, 80, 80, 80, 255);
    for (int a = 0; a < 360; a++) {
        float rad = a * M_PI / 180.0f;
        int x = cx + (int)(rayon * cos(rad));
        int y = cy + (int)(rayon * sin(rad));
        SDL_RenderDrawPoint(r, x, y);
    }

    /* arc colore = temps restant */
    int deg = (int)(pct * 360.0f);
    SDL_SetRenderDrawColor(r,
                           rouge_c, vert_c, 50, 255);
    for (int a = -90; a < -90 + deg; a++) {
        float rad = a * M_PI / 180.0f;
        for (int r2 = rayon-5; r2 <= rayon; r2++) {
            int x = cx + (int)(r2 * cos(rad));
            int y = cy + (int)(r2 * sin(rad));
            SDL_RenderDrawPoint(r, x, y);
        }
    }

    /* carre qui tourne = animation */
    float ra = e->angle_anim;
    int ax = cx + (int)((rayon+8) * cos(ra));
    int ay = cy + (int)((rayon+8) * sin(ra));
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_Rect carre = {ax-4, ay-4, 8, 8};
    SDL_RenderFillRect(r, &carre);

    /* clignoter si < 5 secondes */
    if (restant <= 5) {
        Uint32 tick = SDL_GetTicks() / 200;
        if (tick % 2 == 0) {
            SDL_SetRenderDrawBlendMode(r,
                SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(r,
                220, 50, 50, 80);
            SDL_Rect alerte = {cx-rayon-5,
                               cy-rayon-5,
                               (rayon+5)*2,
                               (rayon+5)*2};
            SDL_RenderFillRect(r, &alerte);
        }
    }
}

/* liberation */
void libererEnigme(Enigme *e)
{
    if (e->tex_question)
        SDL_DestroyTexture(e->tex_question);
    if (e->tex_rep1)
        SDL_DestroyTexture(e->tex_rep1);
    if (e->tex_rep2)
        SDL_DestroyTexture(e->tex_rep2);
    if (e->tex_rep3)
        SDL_DestroyTexture(e->tex_rep3);
}
