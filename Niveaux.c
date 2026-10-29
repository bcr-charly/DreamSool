#include "Niveaux.h"

const Niveau Table_des_niveaux[] = {
    {
        .id = 1,
        .nom = "Niveau 1 : la Foret",
        .pv_boss = 200,
        .attaqueP_boss = 15,
        .attaqueS_boss = 5,
        .defenseP_boss = 5,
        .defenseS_boss = 5,
        .type_boss = TERRE,
        .nb_coups = 25,
        .delai_entrave = 3,
        .delai_attaque = 1,
        .lots_entraves = {
            { ENTRAVE_PIERRE, 1 },
            { ENTRAVE_METAL, 1 },
        },
        .nb_lots_entraves = 2
    }
};

const int Nb_niveaux = sizeof(Table_des_niveaux) / sizeof(Table_des_niveaux[0]);
