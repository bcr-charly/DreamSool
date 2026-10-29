#ifndef GRILLE_H
#define GRILLE_H

#include "Livre-des-ames.h"
#include "Equipe.h"

#define TAILLE_GRILLE 6

#define SOOL_PIERRE 100
#define SOOL_METAL 101

#define TOURS_METAL 3

typedef enum { RIEN = 0, ENTRAVE_PIERRE, ENTRAVE_METAL } Entrave;

typedef struct {
    int sool;
    Entrave entrave;
    int saignement;
    int tours_metal;
} Case;

typedef struct {
    Case cases[TAILLE_GRILLE][TAILLE_GRILLE];
} Grille;

void generer_grille(Grille *grille, const Equipe *equipe);
void afficher_grille(const Grille *grille, const Equipe *equipe);
void echanger_cases(Grille *grille, int l1, int c1, int l2, int c2);
int faire_descendre(Grille *grille);
int remplir_grille(Grille *grille, const Equipe *equipe);
int est_vide(const Case *c);
int couleur_case(const Case *c);
void vider_case(Case *c);

#endif
