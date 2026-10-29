#ifndef COMBAT_H
#define COMBAT_H

#include "Grille.h"
#include "Niveaux.h"
#include "Alignements.h"

int calculer_degats(const Equipe *equipe, const ResultatAlignement *res, const Niveau *niveau);
int resoudre_tour(Grille *grille, const Equipe *equipe, const Niveau *niveau, int *pv_boss);

#endif
