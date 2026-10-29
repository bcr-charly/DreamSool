#include "Livre-des-ames.h"

const Sool Livre_des_ames[] = {
    {
        .nom = "Ame de l'Eau",
        .id = 1,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .croissance_attaqueP = 1, .croissance_attaqueS = 1,
        .croissance_defenseP = 1,  .croissance_defenseS = 2,
        .croissance_pv = 10,
        .type = &Table_des_types[EAU],
        .image = "ame_eau.png"
    },
    {
        .nom = "Ame du FEU",
        .id = 2,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .croissance_attaqueP = 1, .croissance_attaqueS = 1,
        .croissance_defenseP = 1,  .croissance_defenseS = 2,
        .croissance_pv = 10,
        .type = &Table_des_types[FEU],
        .image = "ame_feu.png"
    },
    {
        .nom = "Ame Vegetale",
        .id = 3,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .croissance_attaqueP = 1, .croissance_attaqueS = 1,
        .croissance_defenseP = 1,  .croissance_defenseS = 2,
        .croissance_pv = 10,
        .type = &Table_des_types[PLANTE],
        .image = "ame_plante.png"
    },
    {
        .nom = "Ame de la Terre",
        .id = 4,
        .attaqueP = 10, .attaqueS = 5,
        .defenseP = 8,  .defenseS = 12,
        .pv = 100,
        .croissance_attaqueP = 1, .croissance_attaqueS = 1,
        .croissance_defenseP = 1,  .croissance_defenseS = 2,
        .croissance_pv = 10,
        .type = &Table_des_types[TERRE],
        .image = "ame_terre.png"
    },
    {
        .nom = "Ame de l'Air",
        .id = 5,
        .attaqueP = 12, .attaqueS = 6,
        .defenseP = 6,  .defenseS = 10,
        .pv = 110,
        .croissance_attaqueP = 2, .croissance_attaqueS = 1,
        .croissance_defenseP = 1,  .croissance_defenseS = 1,
        .croissance_pv = 10,
        .type = &Table_des_types[AIR],
        .image = "ame_air.png"
    },
    {
        .nom = "Ame Energetique",
        .id = 6,
        .attaqueP = 8, .attaqueS = 12,
        .defenseP = 6, .defenseS = 10,
        .pv = 90,
        .croissance_attaqueP = 1, .croissance_attaqueS = 2,
        .croissance_defenseP = 1,  .croissance_defenseS = 1,
        .croissance_pv = 8,
        .type = &Table_des_types[ENERGIE],
        .image = "ame_energie.png"
    }
};

const int Nb_sools = sizeof(Livre_des_ames) / sizeof(Livre_des_ames[0]);
