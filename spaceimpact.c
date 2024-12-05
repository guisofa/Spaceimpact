#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>

#include "background.h"
#include "player.h"
#include "inimigos.h"
#include "chefes.h"
#include "colisoes.h"
#include "drops.h"

#define X_SCREEN 1000
#define Y_SCREEN 500

bala* balas_perdidas = NULL;

void desenha_tela(background* bg, player* p, inimigo* i, drop* drops, spawner* s, baller* b, ALLEGRO_FONT* font) {
	al_draw_bitmap(bg->poeira.bitmap, bg->poeira.x, 0, ALLEGRO_FLIP_HORIZONTAL * bg->flipped);
	al_draw_bitmap(bg->poeira.bitmap, bg->poeira.x + X_SCREEN, 0, ALLEGRO_FLIP_HORIZONTAL * !bg->flipped);
	al_draw_bitmap(bg->estrelas.bitmap, bg->estrelas.x, 0, 0);
	al_draw_bitmap(bg->estrelas.bitmap, bg->estrelas.x + X_SCREEN, 0, 0);
	al_draw_bitmap(bg->planetas1.bitmap, bg->planetas1.x, 0, 0);
	al_draw_bitmap(bg->planetas1.bitmap, bg->planetas1.x + X_SCREEN, 0, 0);
	al_draw_bitmap(bg->planetas2.bitmap, bg->planetas2.x, 0, 0);
	al_draw_bitmap(bg->planetas2.bitmap, bg->planetas2.x + X_SCREEN, 0, 0);

	for (drop* d = drops; d != NULL; d = (drop*)d->prox) {
		al_draw_bitmap(d->sprite, d->x - DROP_TAM/2, d->y - DROP_TAM/2, 0);
		al_draw_rounded_rectangle(d->x - DROP_TAM/2, d->y - DROP_TAM/2, d->x + DROP_TAM/2, d->y + DROP_TAM/2, 1, 1, al_map_rgb(0, 0, 255), 3);
	}

	for (inimigo* atual = i; atual != NULL; atual = (inimigo*)atual->prox) {
		al_draw_bitmap(atual->sprite, atual->x - atual->largura/2, atual->y - atual->altura/2, ALLEGRO_FLIP_VERTICAL * atual->flags[0]);
		al_draw_rectangle(atual->x - atual->largura/2, atual->y - atual->altura/2, atual->x + atual->largura/2, atual->y + atual->altura/2, al_map_rgb(255, 0, 0), 1);
		if(atual->arma)
			for (bala* b = atual->arma->disparos; b != NULL; b = (bala*) b->prox)
				al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 0, 255));
	}

	for (bala* b = balas_perdidas; b != NULL; b = (bala*) b->prox)
		al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(0, 255, 0));
	
	if (s) {
		if (s->shielder1) {
			if (s->shielder1->linked) {
				al_draw_line(s->shielder1->x, s->shielder1->y, s->x, s->y - s->altura/2, al_map_rgb(0, 255, 255), 2);
			}
			al_draw_bitmap(s->shielder1->sprite, s->shielder1->x - s->shielder1->largura/2, s->shielder1->y - s->shielder1->altura/2, 0);
			for (bala* b = s->shielder1->arma->disparos; b != NULL; b = (bala*) b->prox)
				al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 0, 0));
		}
		if (s->shielder2) {
			if (s->shielder2->linked) {
				al_draw_line(s->shielder2->x, s->shielder2->y, s->x, s->y + s->altura/2, al_map_rgb(0, 255, 255), 2);
			}
			al_draw_bitmap(s->shielder2->sprite, s->shielder2->x - s->shielder2->largura/2, s->shielder2->y - s->shielder2->altura/2, 0);
			for (bala* b = s->shielder2->arma->disparos; b != NULL; b = (bala*) b->prox)
				al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 0, 0));
		}
		al_draw_bitmap(s->sprite, s->x - s->largura/2, s->y - s->altura/2, 0);
		if ((s->shielder1 && s->shielder1->linked) || (s->shielder2 && s->shielder2->linked))
			al_draw_rectangle(s->x - s->largura/2, s->y - s->altura/2, s->x + s->largura/2, s->y + s->altura/2, al_map_rgb(0, 255, 255), 2);
		for (bala* b = s->arma->disparos; b != NULL; b = (bala*) b->prox)
			al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 0, 0));
	}
	if (b) {
		al_draw_bitmap(b->sprite, b->x - b->largura/2, b->y - b->altura/2, 0);
		for (ball* bola = b->bolas; bola != NULL; bola = (ball*)bola->prox)
			al_draw_filled_circle(bola->x, bola->y, bola->raio, al_map_rgb(255, 128, 0));
		for (bala* d = b->arma->disparos; d != NULL; d = (bala*) d->prox)
			al_draw_filled_circle(d->x, d->y, 4, al_map_rgb(255, 0, 0));
	}
	
	if (p->invencibilidade % 20 < 10) {
		if (p->drop_ativo == 1) {
			if ((p->controle->cima && p->controle->baixo) || (!p->controle->cima && !p->controle->baixo))
				al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura, p->altura, p->x, p->y, 0.5, 0.5, 0, 0);
			else if (p->controle->cima) al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura, p->altura, p->x, p->y, 0.5, 0.5, -(ALLEGRO_PI / 15), 0);
			else al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura, p->altura, p->x, p->y, 0.5, 0.5, ALLEGRO_PI / 15, 0);
		}
		else {
			if ((p->controle->cima && p->controle->baixo) || (!p->controle->cima && !p->controle->baixo))
				al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura/2, p->altura/2, p->x, p->y, 1, 1, 0, 0);
			else if (p->controle->cima) al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura/2, p->altura/2, p->x, p->y, 1, 1, -(ALLEGRO_PI / 15), 0);
			else al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura/2, p->altura/2, p->x, p->y, 1, 1, ALLEGRO_PI / 15, 0);
		}
	}
	al_draw_rectangle(p->x - p->largura/2, p->y - p->altura/2, p->x + p->largura/2, p->y + p->altura/2, al_map_rgb(0, 0, 255), 1);
	
	for (bala* b = p->arma->disparos; b != NULL; b = (bala*) b->prox)
		al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 255, 0));

	char vida[32];
	sprintf(vida, "HP = %d", p->hp);
	al_draw_text(font, al_map_rgb(255, 0, 0), 10, 10, 0, vida);

	if (s) {
		al_draw_filled_rectangle (20, Y_SCREEN - 20, 20 + 19 * s->hp, Y_SCREEN - 10, al_map_rgb(255, 0, 0));
		if (b)
			al_draw_filled_rectangle (20, Y_SCREEN - 40, 20 + 19 * b->hp, Y_SCREEN - 30, al_map_rgb(255, 0, 0));
		return;
	}
	if (b)
		al_draw_filled_rectangle (20, Y_SCREEN - 40, 20 + 19 * b->hp, Y_SCREEN - 30, al_map_rgb(255, 0, 0));
}

void atualiza_posicoes(background* bg, player* p, inimigo** i, drop** d, spawner* s, baller* b) {
	move_background(bg, X_SCREEN);
	*d = move_drop(*d, X_SCREEN, Y_SCREEN);

	if (p->controle->cima) move_player(p, 0, X_SCREEN, Y_SCREEN);
	if (p->controle->baixo) move_player(p, 1, X_SCREEN, Y_SCREEN);
	if (p->controle->direita) {
		move_player(p, 2, X_SCREEN, Y_SCREEN);
		if (p->sprite_estado < 5) p->sprite_estado++;
	}
	if (p->controle->esquerda) {
		move_player(p, 3, X_SCREEN, Y_SCREEN);
		if (p->sprite_estado > 0) p->sprite_estado--;
	}
	if (p->tempo_restante) p->tempo_restante--;
	else desativa_drop(p);

	*i = move_inimigo(*i, X_SCREEN, Y_SCREEN);
	balas_perdidas = move_balas(balas_perdidas, X_SCREEN, Y_SCREEN);

	if (s) move_spawner(s, X_SCREEN, Y_SCREEN);
	if (b) move_baller (b, X_SCREEN, Y_SCREEN);


	if (p->arma->cooldown == 0) {
		if (p->controle->atira) player_atira(p);
	}
	else p->arma->cooldown--;
	p->arma->disparos = move_balas(p->arma->disparos, X_SCREEN, Y_SCREEN);

	if (check_collision(p, *i, d, s, b)) {
		p->hp--;
		p->invencibilidade = PLAYER_INVENCIBILITY_FRAMES;
	}

	if (check_hits(p, i, s, b, &balas_perdidas, d)) {
		p->hp--;
		p->invencibilidade = PLAYER_INVENCIBILITY_FRAMES;
	}
}

int main(){
	
	srand(time(NULL));

	al_init();
	al_init_primitives_addon();
	al_init_image_addon();

	al_install_keyboard();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	background* bg = cria_background("fase 1");
	player* player = cria_player(X_SCREEN/4, Y_SCREEN/2);
	drop* drops = NULL;
	inimigo* inimigos = NULL;
	for (int i = 0; i < 50; i++) {
		inimigos = cria_inimigo(X_SCREEN + 20*i, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 2, inimigos);
	}
	spawner* spawner = NULL;
	baller* baller = NULL;	

	ALLEGRO_EVENT event;
	al_start_timer(timer);
	while(1) {
		al_wait_for_event(queue, &event);
		if (spawner && spawner->hp <= 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(0, 255, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "YOU WIN");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");
			al_flip_display();
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		}
		else if (baller && baller->hp <= 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(0, 255, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "YOU WIN");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");
			al_flip_display();
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		}
		else if (player->hp == 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "YOU DIED");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");
			al_flip_display();
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			atualiza_posicoes(bg, player, &inimigos, &drops, spawner, baller);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			desenha_tela(bg, player, inimigos, drops, spawner, baller, font);
			al_flip_display();
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if      (event.keyboard.keycode == ALLEGRO_KEY_A)      joystick_esquerda(player->controle, 1);
			else if (event.keyboard.keycode == ALLEGRO_KEY_D)      joystick_direita(player->controle, 1);
			else if (event.keyboard.keycode == ALLEGRO_KEY_W)      joystick_cima(player->controle, 1);
			else if (event.keyboard.keycode == ALLEGRO_KEY_S)      joystick_baixo(player->controle, 1);
			else if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) joystick_atira(player->controle, 1);
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			if      (event.keyboard.keycode == ALLEGRO_KEY_A)      joystick_esquerda(player->controle, 0);
			else if (event.keyboard.keycode == ALLEGRO_KEY_D)      joystick_direita(player->controle, 0);
			else if (event.keyboard.keycode == ALLEGRO_KEY_W)      joystick_cima(player->controle, 0);
			else if (event.keyboard.keycode == ALLEGRO_KEY_S)      joystick_baixo(player->controle, 0);
			else if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) joystick_atira(player->controle, 0);
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
	}

	bg = libera_background(bg);
	player = destroi_player(player);
	for (inimigo* i = inimigos; i != NULL; i = inimigos) {
		inimigos = (inimigo*)inimigos->prox;
		if (!i->arma) {
			destroi_inimigo(i);
			continue;
		}
		bala* b = i->arma->disparos;
		bala* aux;
		while (b) {
			aux = b;
			b = (bala*)b->prox;
			destroi_bala(aux);
		}
		destroi_inimigo(i);
	}
	bala* b = balas_perdidas;
	bala* aux;
	while (b) {
		aux = b;
		b = (bala*)b->prox;
		destroi_bala(aux);
	}
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}