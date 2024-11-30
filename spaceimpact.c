#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>

#include "background.h"
#include "player.h"
#include "inimigos.h"
#include "chefes.h"

#define X_SCREEN 1000
#define Y_SCREEN 500

bala* balas_perdidas = NULL;

void desenha_tela(background* bg, player* p, inimigo* i, spawner* s, ALLEGRO_FONT* font) {
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
	
	if (p->invencibilidade % 20 < 10) {
		if ((p->controle->cima && p->controle->baixo) || (!p->controle->cima && !p->controle->baixo))
			al_draw_bitmap(p->sprite[p->sprite_estado], p->x - p->largura/2, p->y - p->altura/2, 0);
		else if (p->controle->cima) al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura/2, p->altura/2, p->x, p->y, 1, 1, -(ALLEGRO_PI / 15), 0);
		else al_draw_scaled_rotated_bitmap(p->sprite[p->sprite_estado], p->largura/2, p->altura/2, p->x, p->y, 1, 1, ALLEGRO_PI / 15, 0);
	}
	al_draw_rectangle(p->x - p->largura/2, p->y - p->altura/2, p->x + p->largura/2, p->y + p->altura/2, al_map_rgb(0, 0, 255), 1);
	
	for (bala* b = p->arma->disparos; b != NULL; b = (bala*) b->prox)
		al_draw_filled_circle(b->x, b->y, 4, al_map_rgb(255, 255, 0));

	char vida[32];
	sprintf(vida, "HP = %d", p->hp);
	al_draw_text(font, al_map_rgb(255, 0, 0), 10, 10, 0, vida);

	if (s) {
		al_draw_filled_rectangle (20, Y_SCREEN - 40, 20 + 19 * s->hp, Y_SCREEN - 20, al_map_rgb(255, 0, 0));
	}
}

int check_collision(player* p, inimigo* i, spawner* s) {
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
	if (s) {
		if ((p_teto <= s->y + s->altura/2 && p_chao >= s->y - s->altura/2) && (p_dir >= s->x - s->largura/2 && p_esq <= s->x + s->largura/2))
			return 1;
		if (s->shielder1)
			if ((p_teto <= s->shielder1->y + s->shielder1->altura/2 && p_chao >= s->shielder1->y - s->shielder1->altura/2) && (p_dir >= s->shielder1->x - s->shielder1->largura/2 && p_esq <= s->shielder1->x + s->shielder1->largura/2))
				return 1;
		if (s->shielder2)
			if ((p_teto <= s->shielder2->y + s->shielder2->altura/2 && p_chao >= s->shielder2->y - s->shielder2->altura/2) && (p_dir >= s->shielder2->x - s->shielder2->largura/2 && p_esq <= s->shielder2->x + s->shielder2->largura/2))
				return 1;
	}

	return 0;
}

inimigo* check_hit_on_enemy(bala* b, inimigo* i, unsigned char* hit) {
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
	else i->prox = (struct inimigo*)check_hit_on_enemy(b, (inimigo*)i->prox, hit);
	return i;
}

int check_hit_on_boss(bala* b, spawner* s) {
	if (!s) return 0;

	if ((b->y <= s->y + s->altura/2 && b->y >= s->y - s->altura/2) && (b->x >= s->x - s->largura/2 && b->x <= s->x + s->largura/2)) {
		if (!((s->shielder1 && s->shielder1->linked) || (s->shielder2 && s->shielder2->linked)))
			s->hp--;
		return 1;
	}
	if (s->shielder1) {
		if ((b->y <= s->shielder1->y + s->shielder1->altura/2 && b->y >= s->shielder1->y - s->shielder1->altura/2) && (b->x >= s->shielder1->x - s->shielder1->largura/2 && b->x <= s->shielder1->x + s->shielder1->largura/2)) {
			s->shielder1->hp--;
			if (s->shielder1->hp == 0) {
				if (balas_perdidas) {
					bala* sent = balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)s->shielder1->arma->disparos;
				}
				else {
					balas_perdidas = s->shielder1->arma->disparos;
				}
				s->shielder1 = destroi_shielder(s->shielder1);
			}
			return 1;
		}	
	}
	if (s->shielder2) {
		if ((b->y <= s->shielder2->y + s->shielder2->altura/2 && b->y >= s->shielder2->y - s->shielder2->altura/2) && (b->x >= s->shielder2->x - s->shielder2->largura/2 && b->x <= s->shielder2->x + s->shielder2->largura/2)) {
			s->shielder2->hp--;
			if (s->shielder2->hp == 0) {
				if (balas_perdidas) {
					bala* sent = balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)s->shielder2->arma->disparos;
				}
				else {
					balas_perdidas = s->shielder2->arma->disparos;
				}
				s->shielder2 = destroi_shielder(s->shielder2);
			}
			return 1;
		}	
	}
	return 0;
}

int check_hits(player* p, inimigo** i, spawner* s) {
	unsigned char hit;
	bala* ultimo = p->arma->disparos;

	for (bala* b = p->arma->disparos; b != NULL;) {
		hit = 0;
		*i = check_hit_on_enemy(b, *i, &hit);
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
			continue;
		}
		
		if (check_hit_on_boss(b, s)) {
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
			continue;
		}
		ultimo = b;
		b = (bala*)b->prox;
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

	if (s) {
		ultimo = s->arma->disparos;
		for (bala* b = s->arma->disparos; b != NULL;) {
			if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
				if (ultimo != b) {
					ultimo->prox = b->prox;
					destroi_bala(b);
					return 1;
				}
				else {
					s->arma->disparos = (bala*)b->prox;
					destroi_bala(b);
					return 1;
				}
			}
			ultimo = b;
			b = (bala*)b->prox;
		}

		if (s->shielder1) {
			ultimo = s->shielder1->arma->disparos;
			for (bala* b = s->shielder1->arma->disparos; b != NULL;) {
				if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
					if (ultimo != b) {
						ultimo->prox = b->prox;
						destroi_bala(b);
						return 1;
					}
					else {
						s->shielder1->arma->disparos = (bala*)b->prox;
						destroi_bala(b);
						return 1;
					}
				}
				ultimo = b;
				b = (bala*)b->prox;
			}
		}

		if (s->shielder2) {
			ultimo = s->shielder2->arma->disparos;
			for (bala* b = s->shielder2->arma->disparos; b != NULL;) {
				if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
					if (ultimo != b) {
						ultimo->prox = b->prox;
						destroi_bala(b);
						return 1;
					}
					else {
						s->shielder2->arma->disparos = (bala*)b->prox;
						destroi_bala(b);
						return 1;
					}
				}
				ultimo = b;
				b = (bala*)b->prox;
			}
		}
	}
	return 0;
}

void atualiza_posicoes(background* bg, player* p, inimigo** i, spawner* s) {
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

	move_spawner(s, X_SCREEN, Y_SCREEN);


	if (p->arma->cooldown == 0) {
		if (p->controle->atira) player_atira(p);
	}
	else p->arma->cooldown--;
	p->arma->disparos = move_balas(p->arma->disparos, X_SCREEN, Y_SCREEN);

	if (check_collision(p, *i, s)) {
		p->hp--;
		p->invencibilidade = PLAYER_INVENCIBILITY_FRAMES;
	}

	if (check_hits(p, i, s)) {
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
	inimigo* inimigos = NULL;
	spawner* spawner = cria_spawner(X_SCREEN + 200, Y_SCREEN/2);
	

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
		else if (player->hp == 0) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "YOU DIED");
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");
			al_flip_display();
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			atualiza_posicoes(bg, player, &inimigos, spawner);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			desenha_tela(bg, player, inimigos, spawner, font);
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