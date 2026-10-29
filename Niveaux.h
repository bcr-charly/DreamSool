#ifndef NIVEAUX_H
#define NIVEAUX_H

#include "Table-des-types.h"
#include "Grille.h"

typedef struct {
    Entrave type;
    int nombre;
} LotEntraves;

typedef struct {
    int id;
    const char *nom;
    int pv_boss;
    int attaqueP_boss;
    int attaqueS_boss;
    int defenseP_boss;
    int defenseS_boss;
    TypeId type_boss;
    int nb_coups;
    int delai_entrave;
    int delai_attaque;
    LotEntraves lots_entraves[8];
    int nb_lots_entraves;
} Niveau;

extern const Niveau Table_des_niveaux[];
extern const int Nb_niveaux;

#endif
