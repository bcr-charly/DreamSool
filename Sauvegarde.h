#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "Livre-des-ames.h"

#define NB_MAX_SOOLS_SAUVEGARDE 64
#define CHEMIN_SAUVEGARDE "sauvegarde.txt"

typedef struct {
    int niveau_max;
    int parties_jouees;
    int victoires;
    int defaites;
    int niveaux_sools[NB_MAX_SOOLS_SAUVEGARDE];
    int sools_debloques[NB_MAX_SOOLS_SAUVEGARDE];
} Sauvegarde;

void sauvegarde_init_defaut(Sauvegarde *s);
int charger_sauvegarde(Sauvegarde *s, const char *chemin);
void ecrire_sauvegarde(const Sauvegarde *s, const char *chemin);
int sool_est_debloque(const Sauvegarde *s, int id);
void debloquer_sool(Sauvegarde *s, int id);
int niveau_sool(const Sauvegarde *s, int id);

#endif
