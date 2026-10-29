#ifndef EQUIPE_H
#define EQUIPE_H

#include "Livre-des-ames.h"

#define NB_SOOLS_EQUIPE 4
#define MODE_PHYSIQUE 0
#define MODE_SPECIAL 1

typedef struct {
    const Sool *base;
    int niveau;
    int attaqueP;
    int attaqueS;
    int defenseP;
    int defenseS;
    int pv;
    int pv_max;
    int mode;
} SoolEnJeu;

typedef struct {
    SoolEnJeu sools[NB_SOOLS_EQUIPE];
    int pv_total;
    int pv_max;
} Equipe;

void calculer_stats_sool(SoolEnJeu *s);
void init_sool_en_jeu(SoolEnJeu *s, const Sool *base, int niveau);
void init_equipe(Equipe *equipe, const int ids[NB_SOOLS_EQUIPE], const int niveaux[NB_SOOLS_EQUIPE]);
void recalculer_pv_equipe(Equipe *equipe);

#endif
