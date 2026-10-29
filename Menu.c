#include "Menu.h"
#include "Jeu.h"
#include "Niveaux.h"
#include "Livre-des-ames.h"
#include "Equipe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SOOLS_PAR_PAGE 12

static void vider_ligne(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

static void attendre_entree(void) {
    printf("\nAppuyez sur Entree pour continuer...");
    fflush(stdout);
    vider_ligne();
}

static int lire_ligne(char *buf, int taille) {
    if (!fgets(buf, taille, stdin))
        return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

static int nb_sools_debloques(const Sauvegarde *sauv) {
    int n = 0;
    for (int i = 0; i < Nb_sools; i++)
        if (sool_est_debloque(sauv, i + 1))
            n++;
    return n;
}

static void afficher_menu_niveaux(const Sauvegarde *sauv) {
    int max = sauv->niveau_max;
    if (max > Nb_niveaux)
        max = Nb_niveaux;
    int debut = max - 4;
    if (debut < 1)
        debut = 1;
    printf("\n=== NIVEAUX ===\n");
    printf("Niveau max debloque : %03d\n\n", max);
    for (int i = debut; i <= max; i++)
        printf("  %03d %s\n", i, Table_des_niveaux[i - 1].nom);
    printf("\nEntrez le numero du niveau (ex: 001) ou 0 pour retour.\n");
    printf("> ");
    fflush(stdout);
}

static void afficher_sool_detail(const Sauvegarde *sauv, int id) {
    const Sool *s = &Livre_des_ames[id - 1];
    int niv = niveau_sool(sauv, id);
    SoolEnJeu sej;
    init_sool_en_jeu(&sej, s, niv);
    printf("\n========================================\n");
    printf(" %03d %s  [Niv. %d]\n", s->id, s->nom, niv);
    printf(" Type : %s\n", s->type->nom);
    printf(" PV       : %d / %d\n", sej.pv, sej.pv_max);
    printf(" AttaqueP : %d  (+%d/niv)\n", sej.attaqueP, s->croissance_attaqueP);
    printf(" AttaqueS : %d  (+%d/niv)\n", sej.attaqueS, s->croissance_attaqueS);
    printf(" DefenseP : %d  (+%d/niv)\n", sej.defenseP, s->croissance_defenseP);
    printf(" DefenseS : %d  (+%d/niv)\n", sej.defenseS, s->croissance_defenseS);
    printf("========================================\n");
}

static void menu_liste_sools(const Sauvegarde *sauv) {
    int ids_debloques[Nb_sools];
    int nb = 0;
    for (int i = 0; i < Nb_sools; i++)
        if (sool_est_debloque(sauv, i + 1))
            ids_debloques[nb++] = i + 1;
    if (nb == 0) {
        printf("\nAucune sool debloquee.\n");
        attendre_entree();
        return;
    }
    int page = 0;
    int nb_pages = (nb + SOOLS_PAR_PAGE - 1) / SOOLS_PAR_PAGE;
    for (;;) {
        printf("\n=== LIVRE DES AMES (%d sools, page %d/%d) ===\n", nb, page + 1, nb_pages);
        int debut = page * SOOLS_PAR_PAGE;
        int fin = debut + SOOLS_PAR_PAGE;
        if (fin > nb)
            fin = nb;
        for (int i = debut; i < fin; i++) {
            int id = ids_debloques[i];
            printf("  %03d %s  (Niv. %d)\n", id, Livre_des_ames[id - 1].nom, niveau_sool(sauv, id));
        }
        printf("\nCommandes : n=page suivante, p=page precedente, q=retour\n");
        printf("Entrez l'ID d'une sool (ex: 001) pour voir ses stats.\n");
        printf("> ");
        fflush(stdout);
        char buf[64];
        if (!lire_ligne(buf, sizeof(buf)))
            return;
        if (strcmp(buf, "n") == 0) {
            if (page + 1 < nb_pages)
                page++;
        } else if (strcmp(buf, "p") == 0) {
            if (page > 0)
                page--;
        } else if (strcmp(buf, "q") == 0) {
            return;
        } else {
            int id = atoi(buf);
            if (id >= 1 && id <= Nb_sools && sool_est_debloque(sauv, id))
                afficher_sool_detail(sauv, id);
            else
                printf("ID invalide ou sool non debloquee.\n");
        }
    }
}

static void afficher_equipe(const Equipe *equipe) {
    printf("\n--- Votre equipe ---\n");
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
        const SoolEnJeu *s = &equipe->sools[i];
        const char *mode = (s->mode == MODE_SPECIAL) ? "S" : "P";
        printf("  [%d] %03d %-20s Niv.%d  PV:%d  %s\n",
               i + 1, s->base->id, s->base->nom, s->niveau, s->pv, mode);
    }
}

static int choisir_equipe(const Sauvegarde *sauv, Equipe *equipe, int ids_init[NB_SOOLS_EQUIPE]) {
    int ids[NB_SOOLS_EQUIPE];
    int niveaux[NB_SOOLS_EQUIPE];
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
        ids[i] = ids_init[i];
        niveaux[i] = niveau_sool(sauv, ids[i]);
    }
    int ids_debloques[Nb_sools];
    int nb_debloques = 0;
    for (int i = 0; i < Nb_sools; i++)
        if (sool_est_debloque(sauv, i + 1))
            ids_debloques[nb_debloques++] = i + 1;
    int page = 0;
    int nb_pages = (nb_debloques + SOOLS_PAR_PAGE - 1) / SOOLS_PAR_PAGE;
    for (;;) {
        printf("\n=== CHOIX DE L'EQUIPE ===\n");
        printf("Equipe actuelle :\n");
        for (int i = 0; i < NB_SOOLS_EQUIPE; i++) {
            printf("  [%d] %03d %s\n", i + 1, ids[i], Livre_des_ames[ids[i] - 1].nom);
        }
        printf("\nSools disponibles (page %d/%d) :\n", page + 1, nb_pages);
        int debut = page * SOOLS_PAR_PAGE;
        int fin = debut + SOOLS_PAR_PAGE;
        if (fin > nb_debloques)
            fin = nb_debloques;
        for (int i = debut; i < fin; i++) {
            int id = ids_debloques[i];
            printf("  %03d %s  (Niv. %d)\n", id, Livre_des_ames[id - 1].nom, niveau_sool(sauv, id));
        }
        printf("\nCommandes :\n");
        printf("  <ID> <position>  -> placer une sool (ex: 001 2)\n");
        printf("  n / p            -> page suivante / precedente\n");
        printf("  go               -> confirmer et passer au recapitulatif\n");
        printf("  q                -> annuler\n");
        printf("> ");
        fflush(stdout);
        char buf[64];
        if (!lire_ligne(buf, sizeof(buf)))
            return 0;
        if (strcmp(buf, "q") == 0)
            return 0;
        if (strcmp(buf, "go") == 0)
            break;
        if (strcmp(buf, "n") == 0) {
            if (page + 1 < nb_pages)
                page++;
            continue;
        }
        if (strcmp(buf, "p") == 0) {
            if (page > 0)
                page--;
            continue;
        }
        int id, pos;
        if (sscanf(buf, "%d %d", &id, &pos) == 2) {
            if (pos >= 1 && pos <= NB_SOOLS_EQUIPE && id >= 1 && id <= Nb_sools
                && sool_est_debloque(sauv, id)) {
                ids[pos - 1] = id;
                niveaux[pos - 1] = niveau_sool(sauv, id);
                printf("Place : %s en position %d.\n", Livre_des_ames[id - 1].nom, pos);
            } else {
                printf("Invalide.\n");
            }
        } else {
            printf("Commande inconnue.\n");
        }
    }
    init_equipe(equipe, ids, niveaux);
    for (int i = 0; i < NB_SOOLS_EQUIPE; i++)
        ids_init[i] = ids[i];
    return 1;
}

static int menu_recap(const Niveau *niveau, Equipe *equipe) {
    for (;;) {
        printf("\n=== RECAPITULATIF ===\n");
        printf(" Niveau : %03d %s\n", niveau->id, niveau->nom);
        printf(" Boss   : %s (PV %d, type %s)\n",
               Livre_des_ames[niveau->sool_boss - 1].nom,
               niveau->pv_boss, niveau->type_boss == TERRE ? "Terre" :
               niveau->type_boss == EAU ? "Eau" :
               niveau->type_boss == FEU ? "Feu" :
               niveau->type_boss == PLANTE ? "Plante" : "?");
        afficher_equipe(equipe);
        printf("\nCommandes :\n");
        printf("  <position> P  -> passer la sool en mode Physique (ex: 1 P)\n");
        printf("  <position> S  -> passer la sool en mode Special (ex: 2 S)\n");
        printf("  go            -> lancer le niveau\n");
        printf("  q             -> retour\n");
        printf("> ");
        fflush(stdout);
        char buf[64];
        if (!lire_ligne(buf, sizeof(buf)))
            return 0;
        if (strcmp(buf, "q") == 0)
            return 0;
        if (strcmp(buf, "go") == 0)
            return 1;
        int pos;
        char mode;
        if (sscanf(buf, "%d %c", &pos, &mode) == 2) {
            if (pos >= 1 && pos <= NB_SOOLS_EQUIPE) {
                if (mode == 'P' || mode == 'p')
                    equipe->sools[pos - 1].mode = MODE_PHYSIQUE;
                else if (mode == 'S' || mode == 's')
                    equipe->sools[pos - 1].mode = MODE_SPECIAL;
                else
                    printf("Mode invalide (P ou S).\n");
            } else {
                printf("Position invalide (1-4).\n");
            }
        } else {
            printf("Commande inconnue.\n");
        }
    }
}

static void lancer_niveau(const Sauvegarde *sauv, int numero) {
    if (numero < 1 || numero > Nb_niveaux || numero > sauv->niveau_max) {
        printf("Niveau non debloque.\n");
        attendre_entree();
        return;
    }
    const Niveau *niveau = &Table_des_niveaux[numero - 1];
    static int ids_derniere_equipe[NB_SOOLS_EQUIPE] = { 1, 2, 3, 4 };
    Equipe equipe;
    if (!choisir_equipe(sauv, &equipe, ids_derniere_equipe))
        return;
    if (!menu_recap(niveau, &equipe))
        return;
    Sauvegarde *sauv_mutable = (Sauvegarde *) sauv;
    sauv_mutable->parties_jouees++;
    int resultat = jouer_niveau(niveau, &equipe, sauv_mutable);
    ecrire_sauvegarde(sauv_mutable, CHEMIN_SAUVEGARDE);
    if (resultat == VICTOIRE)
        printf("\nNiveau %03d reussi !\n", numero);
    else if (resultat == ABANDON)
        printf("\nNiveau abandonne.\n");
    else
        printf("\nNiveau %03d echoue.\n", numero);
    attendre_entree();
}

static void menu_niveaux(Sauvegarde *sauv) {
    for (;;) {
        afficher_menu_niveaux(sauv);
        char buf[64];
        if (!lire_ligne(buf, sizeof(buf)))
            return;
        if (strcmp(buf, "0") == 0)
            return;
        int num = atoi(buf);
        if (num >= 1 && num <= sauv->niveau_max && num <= Nb_niveaux)
            lancer_niveau(sauv, num);
        else
            printf("Niveau invalide ou non debloque.\n");
    }
}

static void menu_stats(const Sauvegarde *sauv) {
    printf("\n=== STATISTIQUES ===\n");
    printf(" Niveau max atteint : %03d\n", sauv->niveau_max);
    printf(" Parties jouees     : %d\n", sauv->parties_jouees);
    printf(" Victoires          : %d\n", sauv->victoires);
    printf(" Defaites           : %d\n", sauv->defaites);
    printf(" Sools debloques    : %d / %d\n", nb_sools_debloques(sauv), Nb_sools);
    attendre_entree();
}

static void menu_parametres(const Sauvegarde *sauv) {
    printf("\n=== PARAMETRES ===\n");
    printf(" (aucun parametre pour l'instant)\n");
    printf(" Niveaux debloques : %d / %d\n", sauv->niveau_max, Nb_niveaux);
    attendre_entree();
}

void menu_principal(Sauvegarde *sauv) {
    for (;;) {
        printf("\n");
        printf("========================================\n");
        printf("           DREAMSOOL\n");
        printf("========================================\n");
        printf("  1. Niveaux\n");
        printf("  2. Sools\n");
        printf("  3. Parametres\n");
        printf("  4. Stats\n");
        printf("  0. Quitter\n");
        printf("========================================\n");
        printf("> ");
        fflush(stdout);
        char buf[64];
        if (!lire_ligne(buf, sizeof(buf)))
            break;
        if (strcmp(buf, "1") == 0)
            menu_niveaux(sauv);
        else if (strcmp(buf, "2") == 0)
            menu_liste_sools(sauv);
        else if (strcmp(buf, "3") == 0)
            menu_parametres(sauv);
        else if (strcmp(buf, "4") == 0)
            menu_stats(sauv);
        else if (strcmp(buf, "0") == 0)
            break;
        else
            printf("Choix invalide.\n");
    }
    ecrire_sauvegarde(sauv, CHEMIN_SAUVEGARDE);
    printf("\nSauvegarde enregistree. A bientot !\n");
}
