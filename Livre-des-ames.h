#ifndef LIVRE_DES_AMES_H
#define LIVRE_DES_AMES_H

#include "Table-des-types.h"

typedef struct {
    const char *nom;
    int id;
    int attaqueP;
    int attaqueS;
    int defenseP;
    int defenseS;
    int pv;
    const Type *type;
    const char *image;
} Sool;

extern const Sool Livre_des_ames[];
extern const int Nb_sools;

#endif