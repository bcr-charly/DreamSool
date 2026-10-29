#ifndef ALIGNEMENTS_H
#define ALIGNEMENTS_H

#include "Grille.h"

#define MAX_ALIGNEMENTS (TAILLE_GRILLE * TAILLE_GRILLE)

typedef struct {
    int sool;
    float valeur;
    int est_croix;
    int l1, c1, l2, c2;
} Alignement;

typedef struct {
    Alignement liste[MAX_ALIGNEMENTS];
    int nb;
    int marque[TAILLE_GRILLE][TAILLE_GRILLE];
} ResultatAlignement;

float detection_alignement(const Grille *grille, ResultatAlignement *res);
void supprimer_marques(Grille *grille, const ResultatAlignement *res);

#endif