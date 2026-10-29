#ifndef LIVRE_DES_AMES_H
#define LIVRE_DES_AMES_H

#include "Table-des-types.h"

#define NIVEAU_MAX_SOOL 10

typedef struct {
    const char *nom;
    int id;
    int attaqueP;
    int attaqueS;
    int defenseP;
    int defenseS;
    int pv;
    int croissance_attaqueP;
    int croissance_attaqueS;
    int croissance_defenseP;
    int croissance_defenseS;
    int croissance_pv;
    const Type *type;
    const char *image;
} Sool;

extern const Sool Livre_des_ames[];
extern const int Nb_sools;

#endif
