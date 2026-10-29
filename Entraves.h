#ifndef ENTRAVES_H
#define ENTRAVES_H

#include "Grille.h"
#include "Niveaux.h"
#include "Alignements.h"

void poser_entraves(Grille *grille, const Niveau *niveau, int indice_lot, int nb_saignements);
int coup_jouable(const Grille *grille);
int detruire_blocs_adjacents(Grille *grille, const ResultatAlignement *res, const Equipe *equipe);
int vieillir_metaux(Grille *grille);
int tick_saignement(const Grille *grille, const Equipe *equipe);

#endif
