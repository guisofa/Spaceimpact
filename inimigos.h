#ifndef __INIMIGOS__
#define __INIMIGOS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>

#include "arma.h"

typedef struct {
    ALLEGRO_BITMAP* sprite;
    unsigned short altura;
	unsigned short largura;
	short x;
	short y;
    unsigned short hp;
    unsigned char step;
    arma* arma;
    unsigned char tipo;
    int flags[2];
    struct inimigo *prox;
} inimigo;

inimigo* cria_inimigo(short x, short y, int x_max, int y_max, unsigned char tipo, inimigo* prox);
inimigo* destroi_inimigo(inimigo* i);

inimigo* move_inimigo(inimigo* i, int x_max, int y_max);

#endif