#include "header.h"

/* ================================================
   TACHE 1 : INITIALISER ET AFFICHER ENNEMI + ES
   
   EXPLICATION SIMPLE :
   - initEnnemi() = remplir la fiche d'identite de l'ennemi
     (position, vitesse, vie, image...)
   - afficherEnnemi() = dessiner l'ennemi a l'ecran
   - initES() = creer un objet a ramasser
   - afficherES() = dessiner l'objet a l'ecran
   ================================================ */

/* -----------------------------------------------
   Remplir la fiche de l'ennemi
   level=1 → trajectoire droite
   level=2 → trajectoire zigzag
   ----------------------------------------------- */
void initEnnemi(Ennemi *e, SDL_Renderer *r,
                int level, float x, float y)
{
    /* POSITION : ou apparait l'ennemi */
    e->x = x;                    // colonne de depart
    e->y = y;                    // ligne de depart

    /* VITESSE : plus rapide en level 2 */
    e->vx        = (level==1) ? 2.0f : 3.0f;
    e->vy        = 0.0f;         // pas de vitesse verticale au depart
    e->direction = 1;            // commence vers la droite

    /* TAILLE : dimensions affichage en pixels */
    e->largeur = 80;
    e->hauteur = 100;

    /* SANTE : 3 vies au depart */
    e->vie     = 3;
    e->vie_max = 3;
    e->etat    = ENNEMI_VIVANT;  // commence vivant

    /* LEVEL : 1 ou 2 */
    e->level = level;
    e->angle = 0.0f;             // angle pour le zigzag

    /* ANIMATION : commence avec l'animation de marche */
    e->nb_frames      = 4;       // 4 images dans la spritesheet
    e->frame_courante = 0;       // commence a l'image 0
    e->temps_anim     = SDL_GetTicks(); // timer actuel
    e->delai_anim     = 150;     // changer d'image toutes les 150ms
    e->anim_courante  = ANIM_DEPLACEMENT; // animation marche

    /* TRAJECTOIRE selon le level */
    if (level == 1)
        e->trajectoire = TRAJECTOIRE_LINEAIRE; // L1 = ligne droite
    else
        e->trajectoire = TRAJECTOIRE_ZIGZAG;   // L2 = zigzag

    /* CHARGER L'IMAGE (spritesheet) */
    e->spritesheet = IMG_LoadTexture(r, "ennemi_sheet.png");
    if (!e->spritesheet)
        printf("ERREUR: ennemi_sheet.png manquant!\n");

    /* DECOUPER LA SPRITESHEET en 4 frames
       [frame0][frame1][frame2][frame3] ← une ligne de 4 images */
    int fw = 80;  // largeur d'une frame
    int fh = 100; // hauteur d'une frame
    for (int i = 0; i < e->nb_frames; i++) {
        e->frames[i].x = i * fw; // decaler de fw pixels a chaque fois
        e->frames[i].y = 0;      // toujours sur la ligne 0
        e->frames[i].w = fw;
        e->frames[i].h = fh;
    }

    mettreAJourBB(e); // calculer la boite de collision
}

/* -----------------------------------------------
   Dessiner l'ennemi a l'ecran
   ----------------------------------------------- */
void afficherEnnemi(Ennemi *e, SDL_Renderer *r)
{
    /* ne rien afficher si l'ennemi est mort */
    if (e->etat == ENNEMI_NEUTRALISE) return;
    if (!e->spritesheet)              return;

    /* quelle image afficher ? → la frame courante */
    SDL_Rect src = e->frames[e->frame_courante];

    /* ou afficher ? → position actuelle de l'ennemi */
    SDL_Rect dst = {(int)e->x, (int)e->y,
                    e->largeur, e->hauteur};

    /* retourner l'image si l'ennemi va a gauche */
    SDL_RendererFlip flip = (e->direction == -1)
                            ? SDL_FLIP_HORIZONTAL // image retournee
                            : SDL_FLIP_NONE;      // image normale

    /* colorier en rouge si l'ennemi est blesse */
    if (e->etat == ENNEMI_BLESSE)
        SDL_SetTextureColorMod(e->spritesheet, 255, 100, 100);
    else
        SDL_SetTextureColorMod(e->spritesheet, 255, 255, 255);

    /* dessiner l'image */
    SDL_RenderCopyEx(r, e->spritesheet,
                     &src, &dst, 0, NULL, flip);
}

/* -----------------------------------------------
   Creer un element de scene (objet a ramasser)
   ----------------------------------------------- */
void initES(ES *es, SDL_Renderer *r,
            float x, float y, const char *img)
{
    es->x       = x;    // position x de l'objet
    es->y       = y;    // position y de l'objet
    es->largeur = 48;   // taille de l'objet
    es->hauteur = 48;
    es->actif   = 1;    // 1 = l'objet est visible

    /* charger l'image de l'objet */
    es->texture = IMG_LoadTexture(r, img);
    if (!es->texture)
        printf("ERREUR: %s manquant!\n", img);

    mettreAJourBB_ES(es); // calculer la boite collision
}

/* -----------------------------------------------
   Dessiner l'element de scene
   ----------------------------------------------- */
void afficherES(ES *es, SDL_Renderer *r)
{
    /* ne rien afficher si deja ramasse */
    if (!es->actif || !es->texture) return;

    SDL_Rect dst = {(int)es->x, (int)es->y,
                    es->largeur, es->hauteur};
    SDL_RenderCopy(r, es->texture, NULL, &dst);
}

/* -----------------------------------------------
   Creer le joueur
   ----------------------------------------------- */
void initJoueur(Joueur *j, SDL_Renderer *r,
                float x, float y)
{
    j->x              = x;
    j->y              = y;
    j->vx             = 0.0f;
    j->vy             = 0.0f;
    j->largeur        = 64;
    j->hauteur        = 64;
    j->vie            = 3;
    j->vie_max        = 3;
    j->etat           = JOUEUR_VIVANT;
    j->score          = 0;
    j->nb_frames      = 4;
    j->frame_courante = 0;
    j->temps_anim     = SDL_GetTicks();
    j->delai_anim     = 150;

    /* charger spritesheet joueur */
    j->spritesheet = IMG_LoadTexture(r, "joueur_sheet.png");
    if (!j->spritesheet)
        printf("ERREUR: joueur_sheet.png manquant!\n");

    /* decouper en 4 frames */
    int fw = 64, fh = 64;
    for (int i = 0; i < j->nb_frames; i++) {
        j->frames[i].x = i * fw;
        j->frames[i].y = 0;
        j->frames[i].w = fw;
        j->frames[i].h = fh;
    }

    mettreAJourBB_J(j);
}

/* -----------------------------------------------
   Dessiner le joueur + sa barre de vie
   ----------------------------------------------- */
void afficherJoueur(Joueur *j, SDL_Renderer *r,
                    TTF_Font *font)
{
    if (!j->spritesheet) return;

    SDL_Rect src = j->frames[j->frame_courante];
    SDL_Rect dst = {(int)j->x, (int)j->y,
                    j->largeur, j->hauteur};

    /* rouge si blesse */
    if (j->etat == JOUEUR_BLESSE)
        SDL_SetTextureColorMod(j->spritesheet, 255, 100, 100);
    else
        SDL_SetTextureColorMod(j->spritesheet, 255, 255, 255);

    SDL_RenderCopy(r, j->spritesheet, &src, &dst);

    /* barre de vie en haut a gauche */
    int bx = 20, by = 20, bw = 150, bh = 15;

    SDL_SetRenderDrawColor(r, 200, 0, 0, 255);
    SDL_Rect fond = {bx, by, bw, bh};
    SDL_RenderFillRect(r, &fond);

    int vie_w = (j->vie * bw) / j->vie_max;
    SDL_SetRenderDrawColor(r, 0, 220, 0, 255);
    SDL_Rect vie_r = {bx, by, vie_w, bh};
    SDL_RenderFillRect(r, &vie_r);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &fond);

    /* afficher score et vies en texte */
    if (font) {
        char info[64];
        sprintf(info, "Score: %d  Vies: %d",
                j->score, j->vie);
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderUTF8_Blended(
            font, info, blanc);
        if (s) {
            SDL_Texture *t =
                SDL_CreateTextureFromSurface(r, s);
            SDL_Rect d = {600, 10, s->w, s->h};
            SDL_RenderCopy(r, t, NULL, &d);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
    }
}

/* ================================================
   TACHE 2 : DEPLACEMENT ALEATOIRE
   
   EXPLICATION SIMPLE :
   - Level 1 : l'ennemi va de gauche a droite
     comme un ballon qui rebondit sur les murs
   - Level 2 : l'ennemi fait des vagues
     comme un serpent qui ondule
   ================================================ */
void deplacerEnnemi(Ennemi *e, int lw, int lh)
{
    /* ennemi mort = ne bouge plus */
    if (e->etat == ENNEMI_NEUTRALISE) return;

    if (e->trajectoire == TRAJECTOIRE_LINEAIRE)
    {
        /* LEVEL 1 : deplacement en ligne droite
           ← ennemi → ← ennemi → ← ennemi →     */
        e->x += e->vx * e->direction; // avancer

        if (e->x + e->largeur >= lw)  // bord droit touche ?
            e->direction = -1;        // faire demi-tour a gauche
        if (e->x <= 0)                // bord gauche touche ?
            e->direction = 1;         // faire demi-tour a droite
    }
    else
    {
        /* LEVEL 2 : deplacement en zigzag (ondulation)
           l'ennemi avance horizontalement ET
           monte/descend comme une vague (sin)      */
        e->angle += 0.05f;            // incrementer l'angle
        e->x += e->vx * e->direction; // avancer horizontalement
        e->y += (float)(sin(e->angle) * 3.0f); // monter/descendre

        if (e->x + e->largeur >= lw) e->direction = -1;
        if (e->x <= 0)               e->direction =  1;

        /* garder dans l'ecran verticalement */
        if (e->y < 0)                e->y = 0;
        if (e->y + e->hauteur > lh)  e->y = lh - e->hauteur;
    }

    mettreAJourBB(e); // recalculer la boite collision
}

/* deplacement joueur avec les touches */
void deplacerJoueur(Joueur *j, const Uint8 *clavier,
                    int lw, int lh)
{
    if (j->etat == JOUEUR_MORT) return; // mort = immobile

    float v = 4.0f; // vitesse du joueur

    /* fleches ou ZQSD */
    if (clavier[SDL_SCANCODE_LEFT]  ||
        clavier[SDL_SCANCODE_Q]) j->x -= v;
    if (clavier[SDL_SCANCODE_RIGHT] ||
        clavier[SDL_SCANCODE_D]) j->x += v;
    if (clavier[SDL_SCANCODE_UP]    ||
        clavier[SDL_SCANCODE_Z]) j->y -= v;
    if (clavier[SDL_SCANCODE_DOWN]  ||
        clavier[SDL_SCANCODE_S]) j->y += v;

    /* bloquer aux bords de l'ecran */
    if (j->x < 0)               j->x = 0;
    if (j->x + j->largeur > lw) j->x = lw - j->largeur;
    if (j->y < 0)               j->y = 0;
    if (j->y + j->hauteur > lh) j->y = lh - j->hauteur;

    mettreAJourBB_J(j);
}

/* ================================================
   TACHE 3 : ANIMATION ENNEMI
   
   EXPLICATION SIMPLE :
   - La spritesheet contient plusieurs images
   - On change d'image toutes les 150ms
   - Ca donne l'illusion du mouvement
   
   [img0][img1][img2][img3] → img0 → img1 → img2 → img3 → img0...
   ================================================ */
void animerEnnemi(Ennemi *e)
{
    if (e->etat == ENNEMI_NEUTRALISE) return; // mort = pas d'anim

    Uint32 maintenant = SDL_GetTicks();

    /* 150ms ecoule ? → changer d'image */
    if (maintenant - e->temps_anim > (Uint32)e->delai_anim)
    {
        if (e->anim_courante == ANIM_DEPLACEMENT)
            /* marche : cycler les 4 images */
            e->frame_courante =
                (e->frame_courante + 1) % e->nb_frames;
        else
            /* attaque : rester sur l'image 0 */
            e->frame_courante = 0;

        e->temps_anim = maintenant; // reset timer
    }
}

/* animation joueur */
void animerJoueur(Joueur *j)
{
    if (j->etat == JOUEUR_MORT) return;

    Uint32 maintenant = SDL_GetTicks();
    if (maintenant - j->temps_anim > (Uint32)j->delai_anim) {
        j->frame_courante =
            (j->frame_courante + 1) % j->nb_frames;
        j->temps_anim = maintenant;
    }
}

/* ================================================
   TACHE 4 : COLLISION BB
   
   EXPLICATION SIMPLE :
   - Chaque personnage a une boite invisible autour de lui
   - Si 2 boites se touchent → collision !
   - Collision J/Ennemi → joueur perd une vie
   - Collision J/ES → joueur gagne des points
   
   [  boite A  ]     [  boite B  ]  = pas collision
   [  boite A  [CONTACT]  boite B]  = COLLISION !
   ================================================ */

/* detecter si 2 boites se touchent */
int collisionBB(BoundingBox a, BoundingBox b)
{
    /* pas de collision si separes dans une direction */
    return !(a.x + a.w <= b.x ||  // A est a gauche de B
             b.x + b.w <= a.x ||  // B est a gauche de A
             a.y + a.h <= b.y ||  // A est au-dessus de B
             b.y + b.h <= a.y);   // B est au-dessus de A
}

/* recalculer boite ennemi apres deplacement */
void mettreAJourBB(Ennemi *e)
{
    e->bb.x = (int)e->x;
    e->bb.y = (int)e->y;
    e->bb.w = e->largeur;
    e->bb.h = e->hauteur;
}

/* recalculer boite ES */
void mettreAJourBB_ES(ES *es)
{
    es->bb.x = (int)es->x;
    es->bb.y = (int)es->y;
    es->bb.w = es->largeur;
    es->bb.h = es->hauteur;
}

/* recalculer boite joueur */
void mettreAJourBB_J(Joueur *j)
{
    j->bb.x = (int)j->x;
    j->bb.y = (int)j->y;
    j->bb.w = j->largeur;
    j->bb.h = j->hauteur;
}

/* collision joueur / ennemi */
void gererCollision_JE(Joueur *j, Ennemi *e)
{
    if (e->etat == ENNEMI_NEUTRALISE) return; // ennemi mort
    if (j->etat == JOUEUR_MORT)       return; // joueur mort

    if (collisionBB(j->bb, e->bb)) // les 2 boites se touchent ?
    {
        j->vie--;                           // joueur perd 1 vie
        if (j->vie <= 0) {
            j->vie  = 0;
            j->etat = JOUEUR_MORT;          // joueur mort
        } else {
            j->etat = JOUEUR_BLESSE;        // joueur blesse
        }
        e->anim_courante = ANIM_ATTAQUE;    // ennemi attaque
        printf("Touche ! Vie restante: %d\n", j->vie);
    }
    else
    {
        e->anim_courante = ANIM_DEPLACEMENT; // ennemi marche
        if (j->vie > 0) j->etat = JOUEUR_VIVANT;
    }
}

/* collision joueur / element de scene */
void gererCollision_JES(Joueur *j, ES *es)
{
    if (!es->actif)             return; // deja ramasse
    if (j->etat == JOUEUR_MORT) return; // joueur mort

    if (collisionBB(j->bb, es->bb)) // collision ?
    {
        j->score += 10;  // +10 points
        es->actif  = 0;  // l'objet disparait
        printf("Objet ramasse ! Score: %d\n", j->score);
    }
}

/* ================================================
   TACHE 5 : GESTION SANTE + AFFICHAGE
   
   EXPLICATION SIMPLE :
   - vie = 3 → VIVANT  (couleur normale)
   - vie = 1 ou 2 → BLESSE  (teinte rouge)
   - vie = 0 → NEUTRALISE  (disparait)
   
   Barre de vie :
   [■■■■■■■■■■] vie pleine = tout vert
   [■■■■■     ] vie a moitie = vert + rouge
   [          ] vie = 0 = tout rouge
   ================================================ */

/* calculer l'etat selon la vie */
void mettreAJourEtat(Ennemi *e)
{
    if (e->vie <= 0)               // vie 0 → mort
        e->etat = ENNEMI_NEUTRALISE;
    else if (e->vie < e->vie_max / 2) // vie < moitie → blesse
        e->etat = ENNEMI_BLESSE;
    else                           // vie pleine → vivant
        e->etat = ENNEMI_VIVANT;
}

/* enlever des points de vie a l'ennemi */
void blesserEnnemi(Ennemi *e, int degats)
{
    if (e->etat == ENNEMI_NEUTRALISE) return; // deja mort

    e->vie -= degats;              // enlever les degats
    if (e->vie < 0) e->vie = 0;   // minimum 0

    mettreAJourEtat(e);           // recalculer etat

    printf("Ennemi: vie=%d/%d etat=%d\n",
           e->vie, e->vie_max, e->etat);
}

/* dessiner la barre de vie de l'ennemi */
void afficherSante(Ennemi *e, SDL_Renderer *r)
{
    if (e->etat == ENNEMI_NEUTRALISE) return; // mort = pas de barre

    /* position de la barre : au-dessus de l'ennemi */
    int bx = (int)e->x;      // meme x que l'ennemi
    int by = (int)e->y - 12; // 12 pixels au-dessus
    int bw = e->largeur;     // meme largeur que l'ennemi
    int bh = 8;              // 8 pixels de hauteur

    /* fond rouge = vie totale */
    SDL_SetRenderDrawColor(r, 200, 0, 0, 255);
    SDL_Rect fond = {bx, by, bw, bh};
    SDL_RenderFillRect(r, &fond);

    /* partie verte = vie restante */
    int vie_w = (e->vie * bw) / e->vie_max; // calcul proportionnel
    SDL_SetRenderDrawColor(r, 0, 220, 0, 255);
    SDL_Rect vie_r = {bx, by, vie_w, bh};
    SDL_RenderFillRect(r, &vie_r);

    /* bordure blanche autour de la barre */
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &fond);

    /* petit carre colore = indicateur etat
       vert = vivant  orange = blesse  rouge = neutralise */
    SDL_Color c;
    if      (e->etat == ENNEMI_VIVANT)
        c = (SDL_Color){0,   255,   0, 255}; // vert
    else if (e->etat == ENNEMI_BLESSE)
        c = (SDL_Color){255, 165,   0, 255}; // orange
    else
        c = (SDL_Color){255,   0,   0, 255}; // rouge

    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_Rect indic = {bx + bw + 5, by, 8, 8};
    SDL_RenderFillRect(r, &indic);
}

/* ================================================
   LIBERATION MEMOIRE
   ================================================ */

/* liberer ennemi */
void libererEnnemi(Ennemi *e)
{
    if (e->spritesheet) {
        SDL_DestroyTexture(e->spritesheet);
        e->spritesheet = NULL;
    }
}

/* liberer ES */
void libererES(ES *es)
{
    if (es->texture) {
        SDL_DestroyTexture(es->texture);
        es->texture = NULL;
    }
}

/* liberer joueur */
void libererJoueur(Joueur *j)
{
    if (j->spritesheet) {
        SDL_DestroyTexture(j->spritesheet);
        j->spritesheet = NULL;
    }
}
