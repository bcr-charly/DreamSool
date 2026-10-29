#include "Alignements.h"

static float valeur_alignement(int n) {
    if (n == 3) return 1.0f;
    if (n == 4) return 1.5f;
    if (n == 5) return 2.0f;
    return 2.5f;
}

static int meme_case(const Grille *grille, int l, int c, int couleur) {
    if (couleur == 0)
        return 0;
    if (l < 0 || l >= TAILLE_GRILLE || c < 0 || c >= TAILLE_GRILLE)
        return 0;
    return couleur_case(&grille->cases[l][c]) == couleur;
}

static int longueur_ligne(const Grille *grille, int l, int c) {
    int n = 1;
    while (meme_case(grille, l, c + n, couleur_case(&grille->cases[l][c])))
        n++;
    return n;
}

static int longueur_colonne(const Grille *grille, int l, int c) {
    int n = 1;
    while (meme_case(grille, l + n, c, couleur_case(&grille->cases[l][c])))
        n++;
    return n;
}

static void ajouter_alignement(ResultatAlignement *res, int couleur, int l1, int c1, int l2, int c2, int n) {
    Alignement *a = &res->liste[res->nb];
    a->sool = couleur;
    a->valeur = valeur_alignement(n);
    a->est_croix = 0;
    a->l1 = l1; a->c1 = c1; a->l2 = l2; a->c2 = c2;
    res->nb++;
    if (l1 == l2)
        for (int c = c1; c <= c2; c++)
            res->marque[l1][c] = 1;
    else
        for (int l = l1; l <= l2; l++)
            res->marque[l][c1] = 1;
}

static int est_horizontal(const Alignement *a) {
    return a->l1 == a->l2;
}

static int segments_croisent(const Alignement *h, const Alignement *v) {
    return h->l1 >= v->l1 && h->l1 <= v->l2 && v->c1 >= h->c1 && v->c1 <= h->c2;
}

static void detecter_croix(ResultatAlignement *res) {
    for (int i = 0; i < res->nb; i++) {
        if (!est_horizontal(&res->liste[i]))
            continue;
        for (int j = 0; j < res->nb; j++) {
            if (est_horizontal(&res->liste[j]))
                continue;
            int ni = res->liste[i].c2 - res->liste[i].c1;
            int nj = res->liste[j].l2 - res->liste[j].l1;
            if (ni == nj && segments_croisent(&res->liste[i], &res->liste[j])) {
                res->liste[i].est_croix = 1;
                res->liste[j].est_croix = 1;
            }
        }
    }
}

float detection_alignement(const Grille *grille, ResultatAlignement *res) {
    res->nb = 0;
    float total = 0.0f;
    for (int l = 0; l < TAILLE_GRILLE; l++)
        for (int c = 0; c < TAILLE_GRILLE; c++)
            res->marque[l][c] = 0;

    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            if (est_vide(&grille->cases[l][c]))
                continue;
            if (meme_case(grille, l, c - 1, couleur_case(&grille->cases[l][c])))
                continue;
            int n = longueur_ligne(grille, l, c);
            if (n < 3)
                continue;
            ajouter_alignement(res, couleur_case(&grille->cases[l][c]), l, c, l, c + n - 1, n);
            total += valeur_alignement(n);
        }
    }
    for (int l = 0; l < TAILLE_GRILLE; l++) {
        for (int c = 0; c < TAILLE_GRILLE; c++) {
            if (est_vide(&grille->cases[l][c]))
                continue;
            if (meme_case(grille, l - 1, c, couleur_case(&grille->cases[l][c])))
                continue;
            int n = longueur_colonne(grille, l, c);
            if (n < 3)
                continue;
            ajouter_alignement(res, couleur_case(&grille->cases[l][c]), l, c, l + n - 1, c, n);
            total += valeur_alignement(n);
        }
    }
    detecter_croix(res);
    return total;
}

void supprimer_marques(Grille *grille, const ResultatAlignement *res) {
    for (int l = 0; l < TAILLE_GRILLE; l++)
        for (int c = 0; c < TAILLE_GRILLE; c++)
            if (res->marque[l][c])
                vider_case(&grille->cases[l][c]);
}
