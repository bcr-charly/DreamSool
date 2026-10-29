#include "Sauvegarde.h"
#include <stdio.h>

void sauvegarde_init_defaut(Sauvegarde *s) {
    s->niveau_max = 1;
    s->parties_jouees = 0;
    s->victoires = 0;
    s->defaites = 0;
    for (int i = 0; i < NB_MAX_SOOLS_SAUVEGARDE; i++) {
        s->niveaux_sools[i] = 1;
        s->sools_debloques[i] = 0;
    }
    for (int i = 0; i < 4 && i < Nb_sools; i++)
        s->sools_debloques[i] = 1;
}

int charger_sauvegarde(Sauvegarde *s, const char *chemin) {
    sauvegarde_init_defaut(s);
    FILE *f = fopen(chemin, "r");
    if (!f)
        return 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, valeur;
        if (sscanf(ligne, "niveau_max=%d", &valeur) == 1)
            s->niveau_max = valeur;
        else if (sscanf(ligne, "parties_jouees=%d", &valeur) == 1)
            s->parties_jouees = valeur;
        else if (sscanf(ligne, "victoires=%d", &valeur) == 1)
            s->victoires = valeur;
        else if (sscanf(ligne, "defaites=%d", &valeur) == 1)
            s->defaites = valeur;
        else if (sscanf(ligne, "sool_%d_niv=%d", &id, &valeur) == 2) {
            if (id >= 1 && id <= NB_MAX_SOOLS_SAUVEGARDE)
                s->niveaux_sools[id - 1] = valeur;
        } else if (sscanf(ligne, "sool_%d_debloque=%d", &id, &valeur) == 2) {
            if (id >= 1 && id <= NB_MAX_SOOLS_SAUVEGARDE)
                s->sools_debloques[id - 1] = valeur;
        }
    }
    fclose(f);
    return 1;
}

void ecrire_sauvegarde(const Sauvegarde *s, const char *chemin) {
    FILE *f = fopen(chemin, "w");
    if (!f)
        return;
    fprintf(f, "niveau_max=%d\n", s->niveau_max);
    fprintf(f, "parties_jouees=%d\n", s->parties_jouees);
    fprintf(f, "victoires=%d\n", s->victoires);
    fprintf(f, "defaites=%d\n", s->defaites);
    for (int i = 0; i < Nb_sools; i++)
        fprintf(f, "sool_%d_niv=%d\n", i + 1, s->niveaux_sools[i]);
    for (int i = 0; i < Nb_sools; i++)
        fprintf(f, "sool_%d_debloque=%d\n", i + 1, s->sools_debloques[i]);
    fclose(f);
}

int sool_est_debloque(const Sauvegarde *s, int id) {
    if (id < 1 || id > NB_MAX_SOOLS_SAUVEGARDE)
        return 0;
    return s->sools_debloques[id - 1];
}

void debloquer_sool(Sauvegarde *s, int id) {
    if (id < 1 || id > NB_MAX_SOOLS_SAUVEGARDE)
        return;
    s->sools_debloques[id - 1] = 1;
}

int niveau_sool(const Sauvegarde *s, int id) {
    if (id < 1 || id > NB_MAX_SOOLS_SAUVEGARDE)
        return 1;
    return s->niveaux_sools[id - 1];
}
