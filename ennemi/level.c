#include "enigme_aleatoire.h"

// Dans la structure Level ajoute :
Enigme enigme;
int    niveau_enigme; // 0=jeu  1=enigme
int    rep_joueur;    // -1=pas encore répondu
int    score;
int    vies;
Uint32 temps_enigme;
int    duree_enigme;  // secondes

// Dans updateLevel() ajoute :
void updateLevel(Level *lv)
{
    if (lv->niveau_enigme == 0)
    {
        // Jeu normal
        const Uint8 *clavier = SDL_GetKeyboardState(NULL);
        deplacerJoueur(&lv->joueur, clavier, 800, 600);

        for (int i = 0; i < lv->nb_ennemis; i++) {
            deplacerEnnemi(&lv->ennemis[i], 800, 600);
            animerEnnemi(&lv->ennemis[i]);

            // Collision Joueur/Ennemi → déclenche enigme
            if (collisionBB(lv->joueur.bb, lv->ennemis[i].bb)) {
                lv->niveau_enigme = 1;
                lv->enigme        = generer("enigme.txt",
                                            lv->renderer,
                                            lv->font);
                lv->rep_joueur    = -1;
                lv->temps_enigme  = SDL_GetTicks();
            }
        }

        // Collision Joueur/ES
        for (int i = 0; i < lv->nb_es; i++)
            gererCollision_JoueurES(&lv->joueur, &lv->es[i]);
    }
    else if (lv->niveau_enigme == 1)
    {
        // Mode énigme
        Uint32 elapsed = (SDL_GetTicks() - lv->temps_enigme) / 1000;
        int    restant = lv->duree_enigme - (int)elapsed;

        // Timeout
        if (restant <= 0 && lv->rep_joueur == -1) {
            lv->enigme.etat = -1;
            lv->vies--;
        }

        // Réponse donnée
        if (lv->rep_joueur != -1) {
            if (lv->rep_joueur == lv->enigme.numbr) {
                lv->enigme.etat = 1;
                lv->score      += 10;
            } else {
                lv->enigme.etat = -1;
                lv->vies--;
            }
            SDL_Delay(2000);
            // Retour au jeu
            libererEnigme(&lv->enigme);
            lv->niveau_enigme = 0;
            lv->rep_joueur    = -1;
        }

        // Game over
        if (lv->vies <= 0)
            lv->termine = 1;
    }
}
