#ifndef __CHEFES__
#define __CHEFES__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "arma.h"
#include "inimigos.h"

typedef struct{
    ALLEGRO_BITMAP* sprite;
    unsigned short altura;
	unsigned short largura;
	short x;
	short y;
    short hp;
    unsigned char step;
    arma* arma;
    unsigned char linked;
} shielder;


typedef struct  {
    ALLEGRO_BITMAP* sprite;
    unsigned short altura;
	unsigned short largura;
	short x;
	short y;
    unsigned short hp;

    unsigned char step;
    int destino;
    int wait_time;
    unsigned char andando;    

    arma* arma;
    shielder* shielder1;
    int spawn1;
    shielder* shielder2;
    int spawn2;
} spawner;


typedef struct {
    short x;
    short y;
    float raio;
    unsigned char step;
    struct ball* prox;
} ball;

typedef struct {
    ALLEGRO_BITMAP* sprite;
    unsigned short altura;
	unsigned short largura;
	short x;
	short y;
    unsigned short hp;

    unsigned char step;
    int destino;
    int wait_time;
    unsigned char andando; 

    unsigned char cooldown;
    ball* bolas;
    arma* arma;
} baller;

spawner* cria_spawner(short x, short y);
shielder* cria_shielder(spawner* s);

void move_spawner(spawner* s, int x_max, int y_max);

shielder* destroi_shielder(shielder* s);
spawner* destroi_spawner(spawner* s);


ball* cria_bola(short x, short y, ball* prox);
baller* cria_baller(short x, short y);

void move_baller (baller* b, int x_max, int y_max);

ball* destroi_bola(ball* b);

#endif