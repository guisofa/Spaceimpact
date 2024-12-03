#ifndef __DROPS__
#define __DROPS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define DROP_TAM 40
#define DROP_STEP 2

typedef struct {
    ALLEGRO_BITMAP* sprite;
    short x;
    short y;
    unsigned char tag;
    struct drop* ante;
    struct drop* prox;
} drop;

drop* cria_drop (short x, short y, unsigned char tag, drop* prox);
drop* move_drop (drop* d, int x_max, int y_max);
drop* destroi_drop(drop* d);

#endif