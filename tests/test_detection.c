#include "../Alignements.h"
#include <stdio.h>
#include <string.h>

static int echecs = 0;

static void remplir(Grille *g, const int cases[TAILLE_GRILLE][TAILLE_GRILLE]) {
    for (int l = 0; l < TAILLE_GRILLE; l++)
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            g->cases[l][c].sool = cases[l][c];
            g->cases[l][c].entrave = RIEN;
            g->cases[l][c].saignement = 0;
            g->cases[l][c].tours_metal = 0;
        }
}

static void verifier(const char *nom, const Grille *g, float attendu, int nb_attendu) {
    ResultatAlignement res;
    float score = detection_alignement(g, &res);
    if (score != attendu || res.nb != nb_attendu) {
        printf("ECHEC %-32s score=%.1f (attendu %.1f) nb=%d (attendu %d)\n",
               nom, score, attendu, res.nb, nb_attendu);
        echecs++;
    } else {
        printf("OK    %-32s score=%.1f nb=%d\n", nom, score, res.nb);
    }
}

static void verifier_croix(const char *nom, const Grille *g, int segments_attendus) {
    ResultatAlignement res;
    detection_alignement(g, &res);
    int croix = 0;
    for (int i = 0; i < res.nb; i++)
        if (res.liste[i].est_croix)
            croix++;
    if (croix != segments_attendus) {
        printf("ECHEC %-32s segments en croix=%d (attendu %d)\n", nom, croix, segments_attendus);
        echecs++;
    } else {
        printf("OK    %-32s segments en croix=%d\n", nom, croix);
    }
}

int main(void) {
    Grille g;

    const int aucun[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2},
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, aucun);
    verifier("aucun alignement", &g, 0.0f, 0);

    const int ligne3[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2},
        {4,4,4,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, ligne3);
    verifier("ligne de 3", &g, 1.0f, 1);

    const int ligne4[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2},
        {4,4,4,4,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, ligne4);
    verifier("ligne de 4", &g, 1.5f, 1);

    const int ligne5[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2},
        {4,4,4,4,4,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, ligne5);
    verifier("ligne de 5", &g, 2.0f, 1);

    const int colonne3[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,3,3,1,2},
        {1,2,3,1,2,3},
        {2,3,3,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, colonne3);
    verifier("colonne de 3", &g, 1.0f, 1);

    const int croix33[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {2,1,3,1,2,1},
        {1,3,3,3,1,2},
        {2,1,3,1,2,1},
        {1,2,1,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, croix33);
    verifier("croix 3+3", &g, 2.0f, 2);
    verifier_croix("croix 3+3", &g, 2);

    const int croix44[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {2,1,3,1,3,2},
        {1,3,3,3,3,1},
        {2,1,3,1,2,2},
        {1,2,3,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, croix44);
    verifier("croix 4+4", &g, 3.0f, 2);
    verifier_croix("croix 4+4", &g, 2);

    const int croix55[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {2,1,3,1,2,1},
        {1,2,3,2,1,2},
        {3,3,3,3,3,1},
        {1,2,3,2,1,2},
        {2,1,3,1,2,1},
        {1,2,1,2,1,2}
    };
    remplir(&g, croix55);
    verifier("croix 5+5", &g, 4.0f, 2);
    verifier_croix("croix 5+5", &g, 2);

    const int L33[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {3,1,2,1,2,1},
        {3,2,1,2,1,2},
        {3,3,3,1,2,1},
        {1,2,1,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, L33);
    verifier("L 3+3", &g, 2.0f, 2);
    verifier_croix("L 3+3", &g, 2);

    const int t43[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {2,3,3,3,3,1},
        {1,3,1,2,1,2},
        {2,3,2,1,2,1},
        {1,2,1,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, t43);
    verifier("T 4+3 (longueurs differentes)", &g, 2.5f, 2);
    verifier_croix("T 4+3 (longueurs differentes)", &g, 0);

    const int pas_croix[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {2,3,3,3,3,1},
        {1,2,2,1,2,2},
        {2,1,1,2,1,1},
        {1,2,1,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, pas_croix);
    verifier("ligne4 seule (pas de croix)", &g, 1.5f, 1);
    verifier_croix("ligne4 seule (pas de croix)", &g, 0);

    const int double_ligne[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {4,4,4,1,2,3},
        {4,4,4,2,3,1},
        {2,3,1,3,1,2},
        {3,1,2,1,2,3}
    };
    remplir(&g, double_ligne);
    verifier("deux lignes de 3", &g, 2.0f, 2);

    const int croix_et_ligne[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {1,2,1,2,1,2},
        {2,1,3,1,2,1},
        {1,3,3,3,1,2},
        {2,1,3,1,2,1},
        {4,4,4,2,1,2},
        {2,1,2,1,2,1}
    };
    remplir(&g, croix_et_ligne);
    verifier("croix 3+3 + ligne 3", &g, 3.0f, 3);
    verifier_croix("croix 3+3 + ligne 3", &g, 2);

    const int trous[TAILLE_GRILLE][TAILLE_GRILLE] = {
        {0,2,3,1,2,3},
        {2,3,1,0,3,1},
        {3,1,2,3,1,2},
        {1,2,3,1,2,3},
        {2,3,1,2,3,1},
        {3,1,2,3,1,2}
    };
    remplir(&g, trous);
    verifier("trous ignores", &g, 0.0f, 0);

    if (echecs == 0) {
        printf("\nTous les tests passent.\n");
        return 0;
    }
    printf("\n%d test(s) en echec.\n", echecs);
    return 1;
}