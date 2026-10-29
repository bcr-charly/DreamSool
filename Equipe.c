#include "Equipe.h"

void calculer_stats_sool(SoolEnJeu *s) {
    int bonus = s->niveau - 1;
    s->attaqueP = s->base->attaqueP + bonus * s->base->croissance_attaqueP;
    s->attaqueS = s->base->attaqueS + bonus * s->base->croissance_attaqueS;
    s->defenseP = s->base->defenseP + bonus * s->base->croissance_defenseP;
    s->defenseS = s->base->defenseS + bonus * s->base->croissance_defenseS;
    s->pv_max  = s->base->pv + bonus * s->base->croissance_pv;
    s->pv = s->pv_max;
}

void init_sool_en_jeu(SoolEnJeu *s, const Sool *base, int niveau) {
    s->base = base;
    if (niveau < 1)
        niveau = 1;
    if (niveau > NIVEAU_MAX_SOOL)
        niveau = NIVEAU_MAX_SOOL;
    s->niveau = niveau;
    s->mode = MODE_PHYSIQUE;
    calculer_stats_sool(s);
}

void recalculer_pv_equipe(Equipe *equipe) {
    equipe->pv_total = 0;
    equipe->pv_max = 0;
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
        equipe->pv_total += equipe->sools[i].pv;
        equipe->pv_max += equipe->sools[i].pv_max;
    }
}

void init_equipe(Equipe *equipe, const int ids[NB_SOOLS_EQUIPE], const int niveaux[NB_SOOLS_EQUIPE]) {
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
        int id = ids[i];
        if (id < 1)
            id = 1;
        if (id > Nb_sools)
            id = Nb_sools;
        init_sool_en_jeu(&equipe->sools[i], &Livre_des_ames[id - 1], niveaux[i]);
    }
    recalculer_pv_equipe(equipe);
}
