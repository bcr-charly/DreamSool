#include "Grille.h"
#include "Alignements.h"
#include <stdio.h>
#include <stdlib.h>

static const char LETTRES_SOOLS[NB_SOOLS_EQUIPE] = { 'E', 'F', 'P', 'T' };

int est_vide(const Case *c) {
    return c->sool == 0 && c->entrave == RIEN;
}

void vider_case(Case *c) {
    c->sool = 0;
    c->entrave = RIEN;
    c->saignement = 0;
    c->tours_metal = 0;
}

static void init_case(Case *c, int sool) {
    c->sool = sool;
    c->entrave = RIEN;
    c->saignement = 0;
    c->tours_metal = 0;
}

static int grille_stable(const Grille *grille) {
    ResultatAlignement res;
    return detection_alignement(grille, &res) == 0.0f;
}

void init_equipe(Equipe *equipe, const Sool *equipe_sools) {
    equipe->pv_total = 0;
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
        equipe->equipe[i] = &equipe_sools[i];
        equipe->pv_total += equipe_sools[i].pv;
    }
    equipe->pv_max = equipe->pv_total;
}

void generer_grille(Grille *grille, const Equipe *equipe) {
    for (int l = 0; l < TAILLE_GRILLE; l++)
        for (int c = 0; c < TAILLE_GRILLE; c++)
            vider_case(&grille->cases[l][c]);
    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            do {
                init_case(&grille->cases[l][c], rand() % NB_SOOLS_EQUIPE + 1);
            } while (!grille_stable(grille));
        }
    }
    (void) equipe;
}

int couleur_case(const Case *c) {
    if (c->entrave == ENTRAVE_PIERRE)
        return SOOL_PIERRE;
    if (c->entrave == ENTRAVE_METAL)
        return SOOL_METAL;
    return c->sool;
}

void afficher_grille(const Grille *grille) {
    printf("    1 2 3 4 5 6\n");
    printf("  +------------+\n");
    for (int l = 0; l < TAILLE_GRILLE; l++) {
        printf("%d | ", l + 1);
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            const Case *cs = &grille->cases[l][c];
            if (est_vide(cs)) {
                printf(". ");
            } else if (cs->entrave == ENTRAVE_PIERRE) {
                printf("# ");
            } else if (cs->entrave == ENTRAVE_METAL) {
                printf("M ");
            } else if (cs->saignement) {
                printf("%c ", LETTRES_SOOLS[cs->sool - 1] + 32);
            } else {
                printf("%c ", LETTRES_SOOLS[cs->sool - 1]);
            }
        }
        printf("|\n");
    }
    printf("  +------------+\n");
    fflush(stdout);
}

void echanger_cases(Grille *grille, int l1, int c1, int l2, int c2) {
    Case temp = grille->cases[l1][c1];
    grille->cases[l1][c1] = grille->cases[l2][c2];
    grille->cases[l2][c2] = temp;
}

int faire_descendre(Grille *grille) {
    int deplace = 0;
    for (int c = 0; c < TAILLE_GRILLE; c++) {
        for (int l = TAILLE_GRILLE - 1; l > 0; l--) {
            if (est_vide(&grille->cases[l][c]) && !est_vide(&grille->cases[l - 1][c])) {
                grille->cases[l][c] = grille->cases[l - 1][c];
                vider_case(&grille->cases[l - 1][c]);
                deplace = 1;
            }
        }
    }
    return deplace;
}

int remplir_grille(Grille *grille, const Equipe *equipe) {
    int rempli = 0;
    for (int c = 0; c < TAILLE_GRILLE; c++) {
        if (est_vide(&grille->cases[0][c])) {
            init_case(&grille->cases[0][c], rand() % NB_SOOLS_EQUIPE + 1);
            rempli = 1;
        }
    }
    (void) equipe;
    return rempli;
}
