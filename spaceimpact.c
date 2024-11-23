#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>

#include "background.h"
#include "player.h"
#include "inimigos.h"

#define X_SCREEN 1000
#define Y_SCREEN 500

bala* balas_perdidas = NULL;

void desenha_tela(background* bg, player* p, inimigo* i, ALLEGRO_FONT* font) {
	al_draw_bitmap(bg->poeira.bitmap, bg->poeira.x, 0, ALLEGRO_FLIP_HORIZONTAL * bg->flipped);
	al_draw_bitmap(bg->poeira.bitmap, bg->poeira.x + X_SCREEN, 0, ALLEGRO_FLIP_HORIZONTAL * !bg->flipped);
	al_draw_bitmap(bg->estrelas.bitmap, bg->estrelas.x, 0, 0);
	al_draw_bitmap(bg->estrelas.bitmap, bg->estrelas.x + X_SCREEN, 0, 0);
	al_draw_bitmap(bg->planetas1.bitmap, bg->planetas1.x, 0, 0);
	al_draw_bitmap(bg->planetas1.bitmap, bg->planetas1.x + X_SCREEN, 0, 0);
	al_draw_bitmap(bg->planetas2.bitmap, bg->planetas2.x, 0, 0);
	al_draw_bitmap(bg->planetas2.bitmap, bg->planetas2.x + X_SCREEN, 0, 0);

	for (inimigo* atual = i; atual != NULL; atual = (inimigo*)atual->prox) {
		al_draw_bitmap(atual->sprite, atual->x - atual->largura/2, atual->y - atual->altura/2, ALLEGRO_FLIP_VERTICAL * atual->flags[0]);
		al_draw_rectangle(atual->x - atual->largura/2, atual->y - atual->altura/2, atual->x + atual->largura/2, atual->y + atual->altura/2, al_map_rgb(255, 0, 0), 1);
		if(atual->arma)
			for (bala* b = atual->arma->disparos; b != NULL; b = (bala*) b->prox)
				al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 0, 255));
	}
	for (bala* b = balas_perdidas; b != NULL; b = (bala*) b->prox)
		al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(0, 255, 0));
	
	if (p->invencibilidade % 20 < 10)
		al_draw_bitmap(p->sprite[p->sprite_estado], p->x - p->largura/2, p->y - p->altura/2, 0);
	al_draw_rectangle(p->x - p->largura/2, p->y - p->altura/2, p->x + p->largura/2, p->y + p->altura/2, al_map_rgb(0, 0, 255), 1);
	
	for (bala* b = p->arma->disparos; b != NULL; b = (bala*) b->prox)
		al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 255, 0));

	char vida[32];
	sprintf(vida, "HP = %d", p->hp);
	al_draw_text(font, al_map_rgb(255, 0, 0), 10, 10, 0, vida);
}

int check_collision(player* p, inimigo* i) {
	if (p->invencibilidade) {
		p->invencibilidade--;
		return 0;
	}

	short p_teto = p->y - p->altura/2;
	short p_chao = p->y + p->altura/2;
	short p_dir =  p->x + p->largura/2;
	short p_esq =  p->x - p->largura/2;
	for (inimigo* atual = i; atual != NULL; atual = (inimigo*)atual->prox) {
		if ((p_teto <= atual->y + atual->altura/2 && p_chao >= atual->y - atual->altura/2) && (p_dir >= atual->x - atual->largura/2 && p_esq <= atual->x + atual->largura/2))
			return 1;
	}

	return 0;
}

inimigo* check_player_hit(bala* b, inimigo* i, unsigned char* hit) {
	if (!i) return NULL;

	inimigo* aux;
	if ((b->y <= i->y + i->altura/2 && b->y >= i->y - i->altura/2) && (b->x >= i->x - i->largura/2 && b->x <= i->x + i->largura/2)) {
		i->hp--;
		*hit = 1;
		if (i->hp == 0) {
			if (i->arma) {
				if (balas_perdidas) {
					bala* sent = balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)i->arma->disparos;
				}
				else {
					balas_perdidas = i->arma->disparos;
				}
			}
			aux = (inimigo*)i->prox;
			destroi_inimigo(i);
			return aux;
		}
	}
	else i->prox = (struct inimigo*)check_player_hit(b, (inimigo*)i->prox, hit);
	return i;
}

int check_hits(player* p, inimigo** i) {
	unsigned char hit;
	bala* ultimo = p->arma->disparos;

	for (bala* b = p->arma->disparos; b != NULL;) {
		hit = 0;
		*i = check_player_hit(b, *i, &hit);
		if (hit) {
			if (ultimo != b) {
				ultimo->prox = b->prox;
				destroi_bala(b);
				b = (bala*)ultimo->prox;
			}
			else {
				ultimo =(bala*) b->prox;
				if (b == p->arma->disparos)
					p->arma->disparos = ultimo;
				destroi_bala(b);
				b = ultimo;
			}
		}
		else {
			ultimo = b;
			b = (bala*)b->prox;
		}
	}

	if (p->invencibilidade) {
		p->invencibilidade--;
		return 0;
	}
	for (inimigo* atual = *i; atual != NULL; atual = (inimigo*)atual->prox) {
		if (!atual->arma) continue;
		ultimo = atual->arma->disparos;
		for (bala* b = atual->arma->disparos; b != NULL;) {
			if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
				if (ultimo != b) {
					ultimo->prox = b->prox;
					destroi_bala(b);
					return 1;
				}
				else {
					atual->arma->disparos = (bala*)b->prox;
					destroi_bala(b);
					return 1;
				}
			}
			ultimo = b;
			b = (bala*)b->prox;
		}
	}
	ultimo = balas_perdidas;
	for (bala* b = balas_perdidas; b != NULL;) {
		if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
			if (ultimo != b) {
				ultimo->prox = b->prox;
				destroi_bala(b);
				return 1;
			}
			else {
				balas_perdidas = (bala*)b->prox;
				destroi_bala(b);
				return 1;
			}
		}
		ultimo = b;
		b = (bala*)b->prox;
	}
	return 0;
}

void atualiza_posicoes(background* bg, player* p, inimigo** i) {
	move_background(bg, X_SCREEN);

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

	*i = move_inimigo(*i, X_SCREEN, Y_SCREEN);
	balas_perdidas = move_balas(balas_perdidas, X_SCREEN, Y_SCREEN);

	if (p->arma->cooldown == 0) {
		if (p->controle->atira) player_atira(p);
	}
	else p->arma->cooldown--;
	p->arma->disparos = move_balas(p->arma->disparos, X_SCREEN, Y_SCREEN);

	if (check_collision(p, *i)) {
		p->hp--;
		p->invencibilidade = PLAYER_INVENCIBILITY_FRAMES;
	}

	if (check_hits(p, i)) {
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
	player* player = cria_player(X_SCREEN/2, Y_SCREEN/2);
	inimigo* inimigos;
	inimigos = cria_inimigo(X_SCREEN + 100, 125, X_SCREEN, Y_SCREEN, 3, NULL);
	inimigos = cria_inimigo(X_SCREEN + 100, 250, X_SCREEN, Y_SCREEN, 3, inimigos);
	inimigos = cria_inimigo(X_SCREEN + 100, 375, X_SCREEN, Y_SCREEN, 3, inimigos);
	inimigos = cria_inimigo(X_SCREEN + 100, 250, X_SCREEN, Y_SCREEN, 3, inimigos);
	for (int i = 0; i < 15; i++) {
		inimigos = cria_inimigo(X_SCREEN + 200 + 32*i, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 2, inimigos);
	}

	ALLEGRO_EVENT event;
	al_start_timer(timer);
	while(1) {
		al_wait_for_event(queue, &event);
		if (player->hp == 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "YOU DIED");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");
			al_flip_display();
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			atualiza_posicoes(bg, player, &inimigos);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			desenha_tela(bg, player, inimigos, font);
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