#include "balas.h"

bala* cria_bala(short x, short y, unsigned char step, unsigned char trajetoria, bala* prox) {
	bala* novo = malloc(sizeof(bala));

	novo->x = x;
	novo->y = y;
	novo->step = step;
	novo->trajetoria = trajetoria;
	novo->prox = (struct bala*)prox;

	return novo;
}

void destroi_bala(bala *b) {
	free(b);
}

bala* move_balas(bala* b, int x_max, int y_max) {
	if (!b) return NULL;

	bala* aux;
	if (b->trajetoria == 0) {
		b->y -= b->step;
		if (b->y < 0) {
			aux = b;
			b = move_balas((bala*)b->prox, x_max, y_max);
			destroi_bala(aux);
		}
		else b->prox = (struct bala*)move_balas((bala*)b->prox, x_max, y_max);
		return b;
	} else if(b->trajetoria == 1) {
		b->y += b->step;
		if (b->y > y_max) {
			aux = b;
			b = move_balas((bala*)b->prox, x_max, y_max);
			destroi_bala(aux);
		}
		else b->prox = (struct bala*)move_balas((bala*)b->prox, x_max, y_max);
		return b;
	} else if(b->trajetoria == 2) {
		b->x += b->step;
		if (b->x > x_max) {
			aux = b;
			b = move_balas((bala*)b->prox, x_max, y_max);
			destroi_bala(aux);
		}
		else b->prox = (struct bala*)move_balas((bala*)b->prox, x_max, y_max);
		return b;
	}
	b->x -= b->step;
	if (b->x < 0) {
		aux = b;
		b = move_balas((bala*)b->prox, x_max, y_max);
		destroi_bala(aux);
	}
	else b->prox = (struct bala*)move_balas((bala*)b->prox, x_max, y_max);
	return b;
}