#include "Entraves.h"
#include "Alignements.h"
#include "Table-des-types.h"
#include <stdio.h>
#include <stdlib.h>

static void poser_bloc(Grille *grille, Entrave type) {
    int l, c;
    do {
        l = rand() % TAILLE_GRILLE;
        c = rand() % TAILLE_GRILLE;
    } while (est_vide(&grille->cases[l][c]) || grille->cases[l][c].entrave != RIEN);
    grille->cases[l][c].entrave = type;
    if (type == ENTRAVE_METAL)
        grille->cases[l][c].tours_metal = 1;
}

static void poser_saignement(Grille *grille) {
    int l, c;
    do {
        l = rand() % TAILLE_GRILLE;
        c = rand() % TAILLE_GRILLE;
    } while (est_vide(&grille->cases[l][c]) || grille->cases[l][c].entrave != RIEN
             || grille->cases[l][c].saignement);
    grille->cases[l][c].saignement = 1;
}

void poser_entraves(Grille *grille, const Niveau *niveau, int indice_lot, int nb_saignements) {
    if (indice_lot < 0 || indice_lot >= niveau->nb_lots_entraves)
        return;
    const LotEntraves *lot = &niveau->lots_entraves[indice_lot];
    for (int i = 0; i < lot->nombre; i++) {
        if (lot->type == ENTRAVE_PIERRE || lot->type == ENTRAVE_METAL)
            poser_bloc(grille, lot->type);
    }
    for (int i = 0; i < nb_saignements; i++)
        poser_saignement(grille);
}

int coup_jouable(const Grille *grille) {
    Grille copie = *grille;
    for (int l1 = 0; l1 < TAILLE_GRILLE; l1++) {
        for (int c1 = 0; c1 < TAILLE_GRILLE; c1++) {
            for (int l2 = 0; l2 < TAILLE_GRILLE; l2++) {
                for (int c2 = 0; c2 < TAILLE_GRILLE; c2++) {
                    if (l1 == l2 && c1 == c2)
                        continue;
                    if (est_vide(&copie.cases[l1][c1]))
                        continue;
                    echanger_cases(&copie, l1, c1, l2, c2);
                    ResultatAlignement res;
                    int ok = detection_alignement(&copie, &res) > 0.0f;
                    echanger_cases(&copie, l1, c1, l2, c2);
                    if (ok)
                        return 1;
                }
            }
        }
    }
    return 0;
}

static int alignement_en_feu(const ResultatAlignement *res, const Equipe *equipe, int l, int c) {
    for (int i = 0; i < res->nb; i++) {
        const Alignement *a = &res->liste[i];
        int dedans = (a->l1 == a->l2)
            ? (l == a->l1 && c >= a->c1 && c <= a->c2)
            : (c == a->c1 && l >= a->l1 && l <= a->l2);
        if (!dedans)
            continue;
        if (a->sool >= 1 && a->sool <= NB_SOOLS_EQUIPE) {
            TypeId type = equipe->equipe[a->sool - 1]->type->id;
            return type == FEU || type == MAGMA || type == ENERGIE || type == LUMIERE;
        }
    }
    return 0;
}

int detruire_blocs_adjacents(Grille *grille, const ResultatAlignement *res, const Equipe *equipe) {
    int detruits = 0;
    static const int dl[] = { -1, 1, 0, 0 };
    static const int dc[] = { 0, 0, -1, 1 };
    int marque_bloc[TAILLE_GRILLE][TAILLE_GRILLE] = { {0} };

    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            if (!res->marque[l][c])
                continue;
            for (int k = 0; k < 4; k++) {
                int nl = l + dl[k], nc = c + dc[k];
                if (nl < 0 || nl >= TAILLE_GRILLE || nc < 0 || nc >= TAILLE_GRILLE)
                    continue;
                Case *cs = &grille->cases[nl][nc];
                if (cs->entrave == RIEN || marque_bloc[nl][nc])
                    continue;
                if (cs->entrave == ENTRAVE_PIERRE) {
                    marque_bloc[nl][nc] = 1;
                    detruits++;
                } else if (cs->entrave == ENTRAVE_METAL && alignement_en_feu(res, equipe, l, c)) {
                    marque_bloc[nl][nc] = 1;
                    detruits++;
                }
            }
        }
    }
    for (int l = 0; l < TAILLE_GRILLE; l++)
        for (int c = 0; c < TAILLE_GRILLE; c++)
            if (marque_bloc[l][c])
                vider_case(&grille->cases[l][c]);
    return detruits;
}

int vieillir_metaux(Grille *grille) {
    int disparus = 0;
    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            Case *cs = &grille->cases[l][c];
            if (cs->entrave != ENTRAVE_METAL)
                continue;
            cs->tours_metal++;
            if (cs->tours_metal >= TOURS_METAL) {
                vider_case(cs);
                disparus++;
            }
        }
    }
    return disparus;
}

int tick_saignement(const Grille *grille, const Equipe *equipe) {
    int degats = 0;
    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            const Case *cs = &grille->cases[l][c];
            if (cs->saignement && cs->sool >= 1 && cs->sool <= NB_SOOLS_EQUIPE)
                degats += equipe->equipe[cs->sool - 1]->attaqueP;
        }
    }
    return degats;
}
