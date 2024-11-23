#include "background.h"

#define STAR_STEP 2
#define PLNT1_STEP 4
#define PLNT2_STEP 7
#define DUST_STEP 1

background* cria_background(char* dir) {
	background* novo = malloc(sizeof(background));
	if (!novo) return NULL;

	char arq[strlen(dir) + 11];

	strcpy(arq, dir);
	strcat(arq, "/poeira.png");
	novo->poeira.bitmap = al_load_bitmap(arq);
	novo->poeira.step = DUST_STEP;
	novo->poeira.x = 0; 

	strcpy(arq, dir);
	strcat(arq, "/estrelas.png");
	novo->estrelas.bitmap = al_load_bitmap(arq);
	novo->estrelas.step = STAR_STEP;
	novo->estrelas.x = 0;

	strcpy(arq, dir);
	strcat(arq, "/planetas1.png");
	novo->planetas1.bitmap = al_load_bitmap(arq);
	novo->planetas1.step = PLNT1_STEP;
	novo->planetas1.x = 0;

	strcpy(arq, dir);
	strcat(arq, "/planetas2.png");
	novo->planetas2.bitmap = al_load_bitmap(arq);
	novo->planetas2.step = PLNT2_STEP;
	novo->planetas2.x = 0;

	novo->flipped = 0;

	return novo;
}

void move_background(background* bg, int x_screen) {
	bg->poeira.x -= bg->poeira.step;
	if (bg->poeira.x + x_screen < 0) {
	    bg->poeira.x = bg->poeira.x + x_screen;
	    bg->flipped = bg->flipped ^ 1;
	}
	bg->estrelas.x -= bg->estrelas.step;
	if (bg->estrelas.x + x_screen < 0) bg->estrelas.x = bg->estrelas.x + x_screen;
	bg->planetas1.x -= bg->planetas1.step;
	if (bg->planetas1.x + x_screen < 0) bg->planetas1.x = bg->planetas1.x + x_screen;
	bg->planetas2.x -= bg->planetas2.step;
	if (bg->planetas2.x + x_screen < 0) bg->planetas2.x = bg->planetas2.x + x_screen;
}

background* libera_background(background* bg) {
	al_destroy_bitmap(bg->estrelas.bitmap);
	al_destroy_bitmap(bg->planetas1.bitmap);
	al_destroy_bitmap(bg->planetas2.bitmap);
	al_destroy_bitmap(bg->poeira.bitmap);
	free(bg);
	return NULL;
}