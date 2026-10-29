#ifndef GRILLE_H
#define GRILLE_H

#include "Livre-des-ames.h"

#define TAILLE_GRILLE 6
#define NB_SOOLS_EQUIPE 4

#define SOOL_PIERRE 5
#define SOOL_METAL 6

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

typedef struct {
    const Sool *equipe[NB_SOOLS_EQUIPE];
    int pv_total;
    int pv_max;
} Equipe;

void init_equipe(Equipe *equipe, const Sool *equipe_sools);
void generer_grille(Grille *grille, const Equipe *equipe);
void afficher_grille(const Grille *grille);
void echanger_cases(Grille *grille, int l1, int c1, int l2, int c2);
int faire_descendre(Grille *grille);
int remplir_grille(Grille *grille, const Equipe *equipe);
int est_vide(const Case *c);
int couleur_case(const Case *c);
void vider_case(Case *c);

#endif
