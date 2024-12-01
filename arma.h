#ifndef __ARMA__
#define __ARMA__
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "balas.h"

typedef struct {
	unsigned char cooldown_inicial;
	unsigned char cooldown;
	bala* disparos;
} arma;	

arma* cria_arma(unsigned char cooldown);
arma* destroi_arma(arma* a);

void atira(arma* a, short x, short y, unsigned char step, unsigned char trajetoria);
void muda_cooldown(arma* a, unsigned char novo_cd);

#endif