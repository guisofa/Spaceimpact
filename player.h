#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"
#include "arma.h"

#define HP_BASE 5
#define SPRITE_ALTURA 27
#define SPRITE_LARGURA 53

#define PLAYER_INVENCIBILITY_FRAMES 60 * 2
#define PLAYER_STEP 10
#define PLAYER_SHOT_COOLDOWN 10
#define PLAYER_SHOT_STEP 8

typedef struct {
	ALLEGRO_BITMAP* sprite;
	unsigned short altura;
	unsigned short largura;
	short x;
	short y;
	unsigned short hp;
	unsigned char invencibilidade;
	joystick* controle;
	arma* arma;
} player;

player* cria_player(short x, short y);
player* destroi_player(player* p);

void move_player(player* p, unsigned char direcao, int x_max, int y_max);
void player_atira(player* p);


#endif