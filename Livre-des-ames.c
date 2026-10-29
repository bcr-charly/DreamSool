#include "Livre-des-ames.h"

const Sool Livre_des_ames[] = {
    {
        .nom = "Ame de l'Eau",
        .id = 1,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .type = &Table_des_types[EAU],
        .image = "ame_eau.png"
    },
    {
        .nom = "Ame du FEU",
        .id = 2,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .type = &Table_des_types[FEU],
        .image = "ame_feu.png"
    },
    {
        .nom = "Ame Vegetale",
        .id = 3,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .type = &Table_des_types[PLANTE],
        .image = "ame_plante.png"
    },
    {
        .nom = "Ame de la Terre",
        .id = 4,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .type = &Table_des_types[TERRE],
        .image = "ame_terre.png"
    },
    
};

const int Nb_sools = sizeof(Livre_des_ames) / sizeof(Livre_des_ames[0]);