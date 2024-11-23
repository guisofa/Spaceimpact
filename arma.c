#include "arma.h"

arma* cria_arma(unsigned char cooldown) {
	arma* novo = malloc(sizeof(arma));
	novo->cooldown_inicial = cooldown;
	novo->cooldown = cooldown; 
	novo->disparos = NULL;

	return novo;
}

void atira(arma* a, short x, short y, unsigned char step, unsigned char trajetoria) {
	a->disparos = cria_bala(x, y, step, trajetoria, a->disparos);
	a->cooldown = a->cooldown_inicial;
}

arma* destroi_arma(arma* a) {
	if (!a) return NULL;
	free(a);
	return NULL;
}
