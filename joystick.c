#include "joystick.h"

joystick* cria_joystick(){

	joystick *novo = malloc(sizeof(joystick));
	if (!novo) return NULL;

	novo->direita = 0;
	novo->esquerda = 0;
	novo->cima = 0;
	novo->baixo = 0;
	novo->atira = 0;

	return novo;
}

void destroi_joystick(joystick *js) {
	free(js);
}

void joystick_esquerda(joystick *js, unsigned char val) {
	js->esquerda = val;
}

void joystick_direita(joystick *js, unsigned char val) {
	js->direita = val;
}

void joystick_cima(joystick *js, unsigned char val) {
	js->cima = val;
}

void joystick_baixo(joystick *js, unsigned char val) {
	js->baixo = val;
}

void joystick_atira(joystick *js, unsigned char val) {
	js->atira = val;
}