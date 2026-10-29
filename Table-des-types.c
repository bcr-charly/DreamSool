#include "Table-des-types.h"

const Type Table_des_types[NB_TYPES] = {
    [EAU]     = { .id = 0,  .nom = "Eau",     .image = "eau.png"     },
    [FEU]     = { .id = 1,  .nom = "Feu",     .image = "feu.png"     },
    [PLANTE]  = { .id = 2,  .nom = "Plante",  .image = "plante.png"  },
    [TERRE]   = { .id = 3,  .nom = "Terre",   .image = "terre.png"   },
    [AIR]     = { .id = 4,  .nom = "Air",     .image = "air.png"     },
    [ENERGIE] = { .id = 5,  .nom = "Energie", .image = "energie.png" },
    [LUMIERE] = { .id = 6,  .nom = "Lumiere", .image = "lumiere.png" },
    [OBSCURE] = { .id = 7,  .nom = "Obscure", .image = "obscure.png" },
    [METAL]   = { .id = 8,  .nom = "Metal",   .image = "metal.png"   },
    [POISON]  = { .id = 9, .nom = "Poison",  .image = "poison.png"  },
    [PLASMA]  = { .id = 10, .nom = "Plasma",  .image = "plasma.png"  },
    [SPECTRE] = { .id = 11, .nom = "Spectre", .image = "spectre.png" },
    [MAGMA]   = { .id = 12, .nom = "Magma",   .image = "magma.png"   },
    [SON]     = { .id = 13, .nom = "Son",     .image = "son.png"     },
    [GLACE]   = { .id = 14, .nom = "Glace",   .image = "glace.png"   },
    [SABLE]   = { .id = 15, .nom = "Sable",   .image = "sable.png"   },
    [SPORE]   = { .id = 16, .nom = "Spore",   .image = "spore.png"   },
    [ABYSSE]  = { .id = 17, .nom = "Abysse",  .image = "abysse.png"  },
};

float Affinites[NB_TYPES][NB_TYPES];

void init_affinites(void) {
    for (int i = 0; i < NB_TYPES; i++)
        for (int j = 0; j < NB_TYPES; j++)
            Affinites[i][j] = 1.0f;
    Affinites[EAU][FEU] = 2.0f;
    Affinites[FEU][PLANTE] = 2.0f;
    Affinites[PLANTE][EAU] = 2.0f;
    Affinites[FEU][EAU] = 0.5f;
    Affinites[PLANTE][FEU] = 0.5f;
    Affinites[EAU][PLANTE] = 0.5f;
}

float multiplicateur_affinite(int attaquant, int defenseur) {
    return Affinites[attaquant][defenseur];
}