#include "player.h"

player* cria_player(short x, short y) {
	player* novo = malloc(sizeof(player));
	if (!novo) return NULL;

	novo->x = x;
	novo->y = y;
	novo->altura = SPRITE_ALTURA;
	novo->largura = SPRITE_LARGURA;
	novo->hp = HP_BASE;
	novo->invencibilidade = 0;

	novo->controle = cria_joystick();
	novo->arma = cria_arma(PLAYER_SHOT_COOLDOWN);

	novo->sprite = al_load_bitmap("nave.png");

	return novo;
}

void move_player(player* p, unsigned char direcao, int x_max, int y_max) {
	if (direcao == 0) {
		p->y -= PLAYER_STEP;
		if (p->y - p->altura/2 < 0)
			p->y = 0 + p->altura/2;
		return;
	}

	if (direcao == 1) {
		p->y += PLAYER_STEP;
		if (p->y + p->altura/2 > y_max)
			p->y = y_max - p->altura/2;
		return;
	}

	if (direcao == 2) {
		p->x += PLAYER_STEP;
		if (p->x + p->largura/2 > x_max)
			p->x = x_max - p->largura/2;
		return;
	}

	if (direcao == 3) {
		p->x -= PLAYER_STEP;
		if (p->x - p->largura/2 < 0)
			p->x = 0 + p->largura/2;
		return;
	}
}
void player_atira(player* p) {
	atira(p->arma, p->x + p->largura/2, p->y, PLAYER_SHOT_STEP, 2);
}

player* destroi_player(player* p) {
	destroi_joystick(p->controle);
	al_destroy_bitmap(p->sprite);
	bala* b = p->arma->disparos;
	bala* aux;
	while (b) {
		aux = b;
		b = (bala*)b->prox;
		destroi_bala(aux);
	}
	destroi_arma(p->arma);
	free(p);
	return NULL;
}