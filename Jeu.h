#ifndef JEU_H
#define JEU_H

#include "Grille.h"
#include "Niveaux.h"
#include "Equipe.h"
#include "Sauvegarde.h"

#define ABANDON 0
#define VICTOIRE 1
#define DEFAITE_COUP 2
#define DEFAITE_PV 3

int jouer_niveau(const Niveau *niveau, Equipe *equipe, Sauvegarde *sauv);

#endif
