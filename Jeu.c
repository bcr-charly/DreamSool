#include "Jeu.h"
#include "Combat.h"
#include "Entraves.h"
#include "Alignements.h"
#include <stdio.h>

static void vider_ligne(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

static int lire_entier(int *valeur) {
    int retour = scanf("%d", valeur);
    if (retour == EOF)
        return -1;
    if (retour != 1) {
        vider_ligne();
        return 0;
    }
    return 1;
}

static void afficher_etat_combat(const Equipe *equipe, const Niveau *niveau, int pv_boss,
                                 int coups_restants, int coups_avant_entrave) {
    printf("\n========================================\n");
    printf(" Boss : %s | PV : %d / %d\n", niveau->nom, pv_boss, niveau->pv_boss);
    printf(" Equipe : PV %d / %d\n", equipe->pv_total, equipe->pv_max);
    printf(" Coups restants : %d | Coups avant entrave : %d\n",
           coups_restants, coups_avant_entrave);
    printf("========================================\n");
}

static int lire_coup(int *l1, int *c1, int *l2, int *c2) {
    printf("\nEntrez votre coup : (ligne1 col1) entree (ligne2 col2) entree\n");
    printf("Exemple : 4 5 entree 4 4 entree.  0 0 0 0 pour abandonner.\n");
    fflush(stdout);
    int r1 = lire_entier(l1);
    if (r1 <= 0)
        return r1;
    int r2 = lire_entier(c1);
    if (r2 <= 0)
        return r2;
    int r3 = lire_entier(l2);
    if (r3 <= 0)
        return r3;
    int r4 = lire_entier(c2);
    if (r4 <= 0)
        return r4;
    if (*l1 == 0 && *c1 == 0 && *l2 == 0 && *c2 == 0)
        return -1;
    (*l1)--; (*c1)--; (*l2)--; (*c2)--;
    if (*l1 < 0 || *l1 >= TAILLE_GRILLE || *c1 < 0 || *c1 >= TAILLE_GRILLE ||
        *l2 < 0 || *l2 >= TAILLE_GRILLE || *c2 < 0 || *c2 >= TAILLE_GRILLE)
        return 0;
    if (*l1 == *l2 && *c1 == *c2)
        return 0;
    return 1;
}

static void appliquer_entrave_boss(Grille *grille, const Equipe *equipe, const Niveau *niveau,
                                   int *indice_lot) {
    poser_entraves(grille, niveau, *indice_lot, 1);
    (*indice_lot)++;
    if (*indice_lot >= niveau->nb_lots_entraves)
        *indice_lot = 0;
    printf("\nLe boss lance une entrave !\n");
    afficher_grille(grille);
    while (!coup_jouable(grille)) {
        printf("Aucun coup jouable : regeneration de la grille (entraves retirees).\n");
        generer_grille(grille, equipe);
        afficher_grille(grille);
    }
}

int jouer_niveau(const Niveau *niveau, const Sool *equipe_sools) {
    Equipe equipe;
    Grille grille;
    int pv_boss = niveau->pv_boss;
    int coups_restants = niveau->nb_coups;
    int coups_avant_entrave = niveau->delai_entrave;
    int indice_lot = 0;
    int tour = 0;

    init_affinites();
    init_equipe(&equipe, equipe_sools);
    generer_grille(&grille, &equipe);

    printf("Debut du %s !\n", niveau->nom);
    afficher_etat_combat(&equipe, niveau, pv_boss, coups_restants, coups_avant_entrave);
    afficher_grille(&grille);

    while (pv_boss > 0 && coups_restants > 0 && equipe.pv_total > 0) {
        if (!coup_jouable(&grille)) {
            printf("Aucun coup jouable : regeneration de la grille (entraves retirees).\n");
            generer_grille(&grille, &equipe);
            afficher_grille(&grille);
        }

        int l1, c1, l2, c2;
        int saisie = lire_coup(&l1, &c1, &l2, &c2);
        if (saisie < 0) {
            printf("Partie abandonnee.\n");
            return ABANDON;
        }
        if (saisie == 0) {
            printf("Coup invalide (hors grille ou meme case). Reessayez.\n");
            continue;
        }

        echanger_cases(&grille, l1, c1, l2, c2);
        ResultatAlignement res;
        float score = detection_alignement(&grille, &res);
        if (score == 0.0f) {
            echanger_cases(&grille, l1, c1, l2, c2);
            printf("Aucun alignement : coup annule, la grille ne change pas.\n");
            continue;
        }

        tour++;
        int degats = resoudre_tour(&grille, &equipe, niveau, &pv_boss);
        coups_restants--;
        coups_avant_entrave--;

        printf("\nDegats infliges au boss ce tour : %d\n", degats);

        int degats_saignement = tick_saignement(&grille, &equipe);
        int degats_boss = niveau->attaqueP_boss;
        equipe.pv_total -= degats_saignement + degats_boss;
        if (degats_saignement > 0)
            printf("Saignement : -%d PV\n", degats_saignement);
        printf("Le boss attaque : -%d PV\n", degats_boss);
        if (equipe.pv_total < 0)
            equipe.pv_total = 0;

        vieillir_metaux(&grille);

        afficher_etat_combat(&equipe, niveau, pv_boss, coups_restants, coups_avant_entrave);
        afficher_grille(&grille);

        if (pv_boss <= 0)
            break;

        if (coups_avant_entrave <= 0 && tour >= 2) {
            appliquer_entrave_boss(&grille, &equipe, niveau, &indice_lot);
            coups_avant_entrave = niveau->delai_entrave;
        }
    }

    if (pv_boss <= 0) {
        printf("\nVICTOIRE ! Le boss est vaincu en %d coups.\n", niveau->nb_coups - coups_restants);
        return VICTOIRE;
    }
    if (equipe.pv_total <= 0) {
        printf("\nDEFAITE : votre equipe n'a plus de points de vie.\n");
        return DEFAITE_PV;
    }
    printf("\nDEFAITE : plus de coups restants, le boss survit avec %d PV.\n", pv_boss);
    return DEFAITE_COUP;
}
