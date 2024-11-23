#ifndef _BACKGROUND_
#define _BACKGROUND_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

struct layer {
	ALLEGRO_BITMAP *bitmap;
	int step;
	int x;
};

struct background {
	struct layer poeira;
	struct layer estrelas;
	struct layer planetas1;
	struct layer planetas2;
	unsigned char flipped;
};
typedef struct background background;

background* cria_background(char* dir);
background* libera_background(background* bg);

void move_background(background* bg, int x_screen);

#endif