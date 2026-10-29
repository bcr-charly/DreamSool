#ifndef TABLE_DES_TYPES_H
#define TABLE_DES_TYPES_H

typedef enum {
    EAU, FEU, PLANTE, TERRE, AIR, ENERGIE,
    LUMIERE, OBSCURE,
    METAL, POISON, PLASMA, SPECTRE, MAGMA, SON, GLACE, SABLE, SPORE, ABYSSE,
    NB_TYPES
} TypeId;

typedef struct {
    int id;
    const char *nom;
    const char *image;
} Type;

extern const Type Table_des_types[NB_TYPES];

void init_affinites(void);
float multiplicateur_affinite(int attaquant, int defenseur);

#endif