#include "Menu.h"
#include "Sauvegarde.h"
#include "Livre-des-ames.h"
#include "Niveaux.h"
#include "Table-des-types.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    unsigned int graine = (unsigned int) time(NULL);
    if (argc > 1)
        graine = (unsigned int) strtoul(argv[1], NULL, 10);
    srand(graine);
    init_affinites();

    Sauvegarde sauv;
    charger_sauvegarde(&sauv, CHEMIN_SAUVEGARDE);

    printf("=== Dreamsool ===\n");
    printf("Livre des ames : %d sools, %d niveaux (graine %u)\n", Nb_sools, Nb_niveaux, graine);

    menu_principal(&sauv);
    return 0;
}
