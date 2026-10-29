#include "Combat.h"
#include "Entraves.h"
#include "Table-des-types.h"
#include <stdio.h>

int calculer_degats(const Equipe *equipe, const ResultatAlignement *res, const Niveau *niveau) {
    float total = 0.0f;
    float totalP = 0.0f;
    float totalS = 0.0f;
    for (int i = 0; i < res->nb; i++) {
        const Alignement *a = &res->liste[i];
        if (a->sool == SOOL_PIERRE || a->sool == SOOL_METAL)
            continue;
        if (a->sool < 1 || a->sool > NB_SOOLS_EQUIPE)
            continue;
        const SoolEnJeu *sool = &equipe->sools[a->sool - 1];
        float attaque = (sool->mode == MODE_SPECIAL) ? sool->attaqueS : sool->attaqueP;
        float mult_affinite = multiplicateur_affinite(sool->base->type->id, niveau->type_boss);
        float mult_croix = a->est_croix ? 1.1f : 1.0f;
        float degats = attaque * mult_affinite * mult_croix * a->valeur;
        total += degats;
        if (sool->mode == MODE_SPECIAL)
            totalS += degats;
        else
            totalP += degats;
    }
    if (total > 0.0f) {
        int mode_dominant = (totalS > totalP) ? MODE_SPECIAL : MODE_PHYSIQUE;
        int defense_boss = (mode_dominant == MODE_SPECIAL) ? niveau->defenseS_boss : niveau->defenseP_boss;
        total -= defense_boss;
    }
    if (total < 0.0f)
        total = 0.0f;
    return (int) total;
}

static void afficher_etat(const char *titre, const Grille *grille, const Equipe *equipe) {
    printf("\n--- %s ---\n", titre);
    afficher_grille(grille, equipe);
    fflush(stdout);
}

int resoudre_tour(Grille *grille, const Equipe *equipe, const Niveau *niveau, int *pv_boss) {
    int total_degats = 0;
    int etape = 0;

    for (;;) {
        ResultatAlignement res;
        float score = detection_alignement(grille, &res);
        if (score > 0.0f) {
            int degats = calculer_degats(equipe, &res, niveau);
            total_degats += degats;
            printf("\nAlignement detecte ! Degats : %d (total du tour : %d)\n", degats, total_degats);
            detruire_blocs_adjacents(grille, &res, equipe);
            supprimer_marques(grille, &res);
            etape++;
            char titre[64];
            snprintf(titre, sizeof(titre), "Etape %d : suppression", etape);
            afficher_etat(titre, grille, equipe);
            continue;
        }
        int chute = faire_descendre(grille);
        int rempli = remplir_grille(grille, equipe);
        if (chute || rempli) {
            afficher_etat("Chute des tuiles", grille, equipe);
            continue;
        }
        break;
    }

    *pv_boss -= total_degats;
    if (*pv_boss < 0)
        *pv_boss = 0;
    return total_degats;
}
