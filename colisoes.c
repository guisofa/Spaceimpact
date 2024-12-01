#include "colisoes.h"

int colisao_player_bola(player* p, ball* b) {
    int xesq = p->x - p->largura/2;
    int xdir = p->x + p->largura/2;
    int ycima = p->y - p->altura/2;
    int ybaixo = p->y + p->altura/2;
    if (b->x > xdir) {
        if (b->y < ycima)
            return (((xdir - b->x)*(xdir - b->x) + (ycima - b->y)*(ycima - b->y)) < (b->raio*b->raio));
        if (b->y > ybaixo)
            return (((xdir - b->x)*(xdir - b->x) + (ybaixo - b->y)*(ybaixo - b->y)) < (b->raio*b->raio));
        return (abs(b->x - xdir) < b->raio);
    }
    if (b->x < xesq) {
        if (b->y < ycima)
            return (((xesq - b->x)*(xesq - b->x) + (ycima - b->y)*(ycima - b->y)) < (b->raio*b->raio));
        if (b->y > ybaixo)
            return (((xesq - b->x)*(xesq - b->x) + (ybaixo - b->y)*(ybaixo - b->y)) < (b->raio*b->raio));
        return (abs(b->x - xesq) < b->raio);
    }
    if (b->y < ycima)
        return (abs(ycima - b->y) < b->raio);
    if (b->y > ybaixo)
        return (abs(ybaixo - b->y) < b->raio);
    return 1;
}


int check_collision(player* p, inimigo* i, spawner* s, baller* b) {
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
    if (b) {
        if ((p_teto <= b->y + b->altura/2 && p_chao >= b->y - b->altura/2) && (p_dir >= b->x - b->largura/2 && p_esq <= b->x + b->largura/2))
			return 1;
    }

	return 0;
}

inimigo* check_hit_on_enemy(bala* b, inimigo* i, bala** balas_perdidas, unsigned char* hit) {
	if (!i) return NULL;

	inimigo* aux;
	if ((b->y <= i->y + i->altura/2 && b->y >= i->y - i->altura/2) && (b->x >= i->x - i->largura/2 && b->x <= i->x + i->largura/2)) {
		i->hp--;
		*hit = 1;
		if (i->hp == 0) {
			if (i->arma) {
				if (*balas_perdidas) {
					bala* sent = *balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)i->arma->disparos;
				}
				else {
					*balas_perdidas = i->arma->disparos;
				}
			}
			aux = (inimigo*)i->prox;
			destroi_inimigo(i);
			return aux;
		}
	}
	else i->prox = (struct inimigo*)check_hit_on_enemy(b, (inimigo*)i->prox, balas_perdidas, hit);
	return i;
}

int check_hit_on_spawner(bala* b, spawner* s, bala** balas_perdidas) {
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
				if (*balas_perdidas) {
					bala* sent = *balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)s->shielder1->arma->disparos;
				}
				else {
					*balas_perdidas = s->shielder1->arma->disparos;
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
				if (*balas_perdidas) {
					bala* sent = *balas_perdidas;
					while (sent->prox) sent = (bala*)sent->prox;
					sent->prox = (struct bala*)s->shielder2->arma->disparos;
				}
				else {
					*balas_perdidas = s->shielder2->arma->disparos;
				}
				s->shielder2 = destroi_shielder(s->shielder2);
			}
			return 1;
		}	
	}
	return 0;
}

int check_hit_on_baller(bala* d, baller* b, bala** balas_perdidas) {
    if (!b) return 0;

    if ((d->y <= b->y + b->altura/2 && d->y >= b->y - b->altura/2) && (d->x >= b->x - b->largura/2 && d->x <= b->x + b->largura/2)) {
        b->hp--;
        return 1;
    }

    ball* ultimo = b->bolas;
    for (ball* bola = b->bolas; bola != NULL;) {
        if (sqrt((bola->x - d->x)*(bola->x - d->x) + (bola->y - d->y)*(bola->y - d->y)) <= bola->raio) {
            atira(b->arma, bola->x, bola->y, 6, 0);
            atira(b->arma, bola->x, bola->y, 6, 1);
            atira(b->arma, bola->x, bola->y, 6, 2);
            atira(b->arma, bola->x, bola->y, 6, 3);
            if (ultimo != bola) {
                ultimo->prox = bola->prox;
                destroi_bola(bola);
                bola = (ball*)ultimo->prox;
            }
            else {
                ultimo =(ball*) bola->prox;
				if (bola == b->bolas)
					b->bolas = ultimo;
				destroi_bola(bola);
				bola = ultimo;
            }
            return 1;
        }
        ultimo = bola;
        bola = (ball*)bola->prox;
    }
    return 0;
}

int check_hits(player* p, inimigo** i, spawner* s, baller* baller, bala** balas_perdidas) {
	unsigned char hit;
	bala* ultimo = p->arma->disparos;

	for (bala* b = p->arma->disparos; b != NULL;) {
		hit = 0;
		*i = check_hit_on_enemy(b, *i, balas_perdidas, &hit);
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
		
		if (check_hit_on_spawner(b, s, balas_perdidas)) {
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

        if (check_hit_on_baller(b, baller, balas_perdidas)) {
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

	ultimo = *balas_perdidas;
	for (bala* b = *balas_perdidas; b != NULL;) {
		if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
			if (ultimo != b) {
				ultimo->prox = b->prox;
				destroi_bala(b);
				return 1;
			}
			else {
				*balas_perdidas = (bala*)b->prox;
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

    if (baller) {
        ultimo = baller->arma->disparos;
		for (bala* b = baller->arma->disparos; b != NULL;) {
			if ((b->y <= p->y + p->altura/2 && b->y >= p->y - p->altura/2) && (b->x >= p->x - p->largura/2 && b->x <= p->x + p->largura/2)) {
				if (ultimo != b) {
					ultimo->prox = b->prox;
					destroi_bala(b);
					return 1;
				}
				else {
					baller->arma->disparos = (bala*)b->prox;
					destroi_bala(b);
					return 1;
				}
			}
			ultimo = b;
			b = (bala*)b->prox;
		}

        ball* ult = baller->bolas;
        for (ball* b = baller->bolas; b != NULL;) {
			if (colisao_player_bola(p, b)) {
                atira(baller->arma, b->x, b->y, 6, 0);
                atira(baller->arma, b->x, b->y, 6, 1);
                atira(baller->arma, b->x, b->y, 6, 2);
                atira(baller->arma, b->x, b->y, 6, 3);
				if (ult != b) {
					ult->prox = b->prox;
					destroi_bola(b);
					return 1;
				}
				else {
					baller->bolas = (ball*)b->prox;
					destroi_bola(b);
					return 1;
				}
			}
			ult = b;
			b = (ball*)b->prox;
		}
    }
	return 0;
}