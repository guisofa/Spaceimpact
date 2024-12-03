#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"
#include "arma.h"
#include "drops.h"

#define HP_BASE 5
#define SPRITE_ALTURA 27
#define SPRITE_LARGURA 53

#define PLAYER_INVENCIBILITY_FRAMES 60 * 2
#define PLAYER_STEP 10
#define PLAYER_SHOT_COOLDOWN 10
#define PLAYER_SHOT_STEP 8

typedef struct {
	ALLEGRO_BITMAP* sprite[6];
	unsigned char sprite_estado;
	unsigned short altura;
	unsigned short largura;
	short x;
	short y;
	unsigned short hp;
	unsigned char invencibilidade;
	unsigned char drop_ativo;
	unsigned char tempo_restante;
	joystick* controle;
	arma* arma;
} player;

player* cria_player(short x, short y);
player* destroi_player(player* p);

void move_player(player* p, unsigned char direcao, int x_max, int y_max);
void player_atira(player* p);
void pega_drop(player* p, drop* d);
void desativa_drop(player* p);


#endif