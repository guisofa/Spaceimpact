#ifndef __BALAS__
#define __BALAS__

#include <stdlib.h>

typedef struct {
	short x;
	short y;
	unsigned char step;
	unsigned char trajetoria;
	struct bala *prox;
} bala;

bala* cria_bala(short x, short y, unsigned char step, unsigned char trajetoria, bala* prox);
void destroi_bala(bala *b);

bala* move_balas(bala* b, int x_max, int y_max);

#endif
