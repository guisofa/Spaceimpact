#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdlib.h>

typedef struct {
	unsigned char direita;
	unsigned char esquerda;
	unsigned char cima;
	unsigned char baixo;
	unsigned char atira;
} joystick;

joystick* cria_joystick();
void destroi_joystick(joystick *js);

void joystick_direita(joystick *js, unsigned char val); 
void joystick_esquerda(joystick *js, unsigned char val);
void joystick_cima(joystick *js, unsigned char val);
void joystick_baixo(joystick *js, unsigned char val);
void joystick_atira(joystick *js, unsigned char val);

#endif 