#include "../Grille.h"
#include "../Alignements.h"
#include "../Combat.h"
#include "../Entraves.h"
#include "../Livre-des-ames.h"
#include "../Niveaux.h"
#include <stdio.h>
#include <stdlib.h>

static int echecs = 0;
static void check(const char *nom, int obtenu, int attendu) {
    if (obtenu != attendu) { printf("ECHEC %-42s obtenu=%d attendu=%d\n", nom, obtenu, attendu); echecs++; }
    else printf("OK    %-42s = %d\n", nom, obtenu);
}
static void poser(Grille *g, int l, int c, int sool) { g->cases[l][c].sool = sool; }

int main(void) {
    Equipe equipe;
    init_equipe(&equipe, Livre_des_ames);
    init_affinites();
    srand(1);
    Grille g;
    ResultatAlignement res;
    const Niveau *niv = &Table_des_niveaux[0];

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,1); poser(&g,2,1,1); poser(&g,2,2,1);
    detection_alignement(&g,&res);
    check("3 EAU vs TERRE = 5", calculer_degats(&equipe,&res,niv), 5);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,2); poser(&g,2,1,2); poser(&g,2,2,2); poser(&g,2,3,2);
    detection_alignement(&g,&res);
    check("4 FEU vs TERRE = 10", calculer_degats(&equipe,&res,niv), 10);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,1,1); poser(&g,2,2,1); poser(&g,2,3,1);
    poser(&g,1,2,1); poser(&g,3,2,1);
    detection_alignement(&g,&res);
    check("croix 3+3 EAU = 17", calculer_degats(&equipe,&res,niv), 17);

    Niveau boss_feu = *niv; boss_feu.type_boss = FEU;
    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,1); poser(&g,2,1,1); poser(&g,2,2,1);
    detection_alignement(&g,&res);
    check("3 EAU vs FEU (x2) = 15", calculer_degats(&equipe,&res,&boss_feu), 15);

    Niveau boss_eau = *niv; boss_eau.type_boss = EAU;
    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,2); poser(&g,2,1,2); poser(&g,2,2,2);
    detection_alignement(&g,&res);
    check("3 FEU vs EAU (x0.5) = 0", calculer_degats(&equipe,&res,&boss_eau), 0);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    g.cases[2][0].entrave = ENTRAVE_PIERRE;
    g.cases[2][1].entrave = ENTRAVE_PIERRE;
    g.cases[2][2].entrave = ENTRAVE_PIERRE;
    detection_alignement(&g,&res);
    check("3 pierres = 0 degat", calculer_degats(&equipe,&res,niv), 0);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,1); poser(&g,2,1,1); poser(&g,2,2,1);
    g.cases[1][2].entrave = ENTRAVE_PIERRE;
    g.cases[5][5].entrave = ENTRAVE_PIERRE;
    detection_alignement(&g,&res);
    check("1 pierre adjacente detruite", detruire_blocs_adjacents(&g,&res,&equipe), 1);
    check("pierre eloignee intacte", g.cases[5][5].entrave == ENTRAVE_PIERRE, 1);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,2); poser(&g,2,1,2); poser(&g,2,2,2);
    g.cases[1][1].entrave = ENTRAVE_METAL;
    detection_alignement(&g,&res);
    check("metal+FEU detruit", detruire_blocs_adjacents(&g,&res,&equipe), 1);
    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,2,0,1); poser(&g,2,1,1); poser(&g,2,2,1);
    g.cases[1][1].entrave = ENTRAVE_METAL;
    detection_alignement(&g,&res);
    check("metal+EAU intact", detruire_blocs_adjacents(&g,&res,&equipe), 0);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    g.cases[3][3].entrave = ENTRAVE_METAL; g.cases[3][3].tours_metal = 1;
    check("metal tour2 reste", vieillir_metaux(&g), 0);
    check("metal tour3 disparait", vieillir_metaux(&g), 1);

    for (int l=0;l<6;l++) for (int c=0;c<6;c++) vider_case(&g.cases[l][c]);
    poser(&g,0,0,1); g.cases[0][0].saignement = 1;
    poser(&g,3,3,2); g.cases[3][3].saignement = 1;
    check("saignement 2 sools = 20", tick_saignement(&g,&equipe), 20);
    faire_descendre(&g);
    check("saignement suit chute (1,0)", g.cases[1][0].saignement == 1, 1);
    check("saignement suit chute (4,3)", g.cases[4][3].saignement == 1, 1);

    generer_grille(&g,&equipe);
    check("grille generee jouable", coup_jouable(&g), 1);

    generer_grille(&g,&equipe);
    detection_alignement(&g,&res);
    check("grille initiale stable (score 0)", res.nb, 0);

    if (echecs == 0) { printf("\nTous les tests finales passent.\n"); return 0; }
    printf("\n%d test(s) en echec.\n", echecs);
    return 1;
}