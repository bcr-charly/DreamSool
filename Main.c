#include "Jeu.h"
#include "Livre-des-ames.h"
#include "Niveaux.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    unsigned int graine = (unsigned int) time(NULL);
    if (argc > 1)
        graine = (unsigned int) strtoul(argv[1], NULL, 10);
    srand(graine);

    printf("=== Dreamsool ===\n");
    printf("Livre des ames : %d sools, %d niveaux (graine %u)\n", Nb_sools, Nb_niveaux, graine);
    for (int i = 0; i < Nb_niveaux; i++) {
        int resultat = jouer_niveau(&Table_des_niveaux[i], Livre_des_ames);
        if (resultat == VICTOIRE)
            printf("Niveau %d reussi !\n", Table_des_niveaux[i].id);
        else
            printf("Niveau %d echoue.\n", Table_des_niveaux[i].id);
    }
    return 0;
}
