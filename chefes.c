#include "chefes.h"

spawner* cria_spawner(short x, short y) {
    spawner* novo = malloc(sizeof(spawner));
    novo->sprite = al_load_bitmap("chefes/spawner.png");
    novo->altura = 110;
    novo->largura = 145;
    novo->x = x;
    novo->y = y;
    novo->hp = 50;
    novo->step = 4;
    novo->destino = 0;
    novo->wait_time = 0;
    novo->andando = 0;
    novo->arma = cria_arma(60);

    novo->shielder1 = NULL;
    novo->spawn1 = 150;
    novo->shielder2 = NULL;
    novo->spawn2 = 150;

    return novo;
}

shielder* cria_shielder(spawner* s) {
    shielder* novo = malloc(sizeof(shielder));
    novo->sprite = al_load_bitmap("chefes/shielder.png");
    novo->altura = 18;
    novo->largura = 53;
    novo->x = s->x;
    novo->y = s->y;
    novo->hp = 2;
    novo->step = 3;
    novo->arma = cria_arma(60);
    novo->linked = 0;

    return novo;
}

void move_shielder(shielder* shielder, spawner* spawner, int n, int x_max, int y_max) {
    if (!shielder) return;
    if (!shielder->linked) {
        if (n == 1) {
            shielder->y -= shielder->step;
            if (abs(shielder->y - (spawner->y - spawner->altura/2 - 50)) < shielder->step * 2) {
                shielder->y = spawner->y - spawner->altura/2 - 50;
                shielder->linked = 1;
            }
        } else {
            shielder->y += shielder->step;
            if (abs(shielder->y - (spawner->y + spawner->altura/2 + 50)) < shielder->step * 2) {
                shielder->y = spawner->y + spawner->altura/2 + 50;
                shielder->linked = 1;
            }
        }
        return;
    }
    if (shielder->arma->cooldown == 0) {
        atira(shielder->arma, shielder->x - shielder->largura/2, shielder->y, 8, 3);
    }
    else shielder->arma->cooldown--;;
    shielder->arma->disparos = move_balas(shielder->arma->disparos, x_max, y_max);

    if (n == 1) {
        if (abs(shielder->y - (spawner->y - spawner->altura/2 - 50)) < shielder->step) {
            shielder->y = spawner->y - spawner->altura/2 - 50;
        }
        else {
            if (shielder->y < (spawner->y - spawner->altura/2 - 50)) shielder->y += shielder->step;
            else shielder->y -= shielder->step;
        }
    }
    else {
        if (abs(shielder->y - (spawner->y + spawner->altura/2 + 50)) < shielder->step) {
            shielder->y = spawner->y + spawner->altura/2 + 50;
        }
        else {
            if (shielder->y < (spawner->y + spawner->altura/2 + 50)) shielder->y += shielder->step;
            else shielder->y -= shielder->step;
        }
    }
}

void move_spawner(spawner* s, int x_max, int y_max) {
    if (s->x + s->largura/2 > x_max - 10) {
        s->x -= s->step;
        return;
    }

    if (!s->destino) s->destino = rng(110, y_max - 110);

    if (!s->shielder1) {
        if (s->spawn1 == 0) {
            s->shielder1 = cria_shielder(s);
            s->spawn1 = 300;
        }
        else  s->spawn1--;
    }
    if (!s->shielder2) {
        if (s->spawn2 == 0) {
            s->shielder2 = cria_shielder(s);
            s->spawn2 = 300;
        }
        else s->spawn2--;
    }

    move_shielder(s->shielder1, s, 1, x_max, y_max);
    move_shielder(s->shielder2, s, 2, x_max, y_max);

    if (s->arma->cooldown == 0) {
            atira(s->arma, s->x - s->largura/2, s->y - s->altura/2 + 5, 5, 3);
            atira(s->arma, s->x - s->largura/2, s->y, 5, 3);
            atira(s->arma, s->x - s->largura/2, s->y + s->altura/2 - 5, 5, 3);
    }
    else s->arma->cooldown--;
    s->arma->disparos = move_balas(s->arma->disparos, x_max, y_max);

    if (s->andando) {
        if (s->destino > s->y ) s->y += s->step;
        else s->y -= s->step;   

        if (abs(s->destino - s->y) > s->step) return;
        s->y = s->destino;
        s->wait_time = 70;
        s->andando = 0;
        return;
    }

    if (!s->wait_time) {
        s->andando = 1;
        s->destino = rng(110, y_max - 110);
    }
    else s->wait_time--;
    
}

shielder* destroi_shielder(shielder* s) {
    if (!s) return NULL;
    al_destroy_bitmap(s->sprite);
    destroi_arma(s->arma);
    free(s);
    return NULL;
}

spawner* destroi_spawner(spawner* s) {
    if (!s) return NULL;
    al_destroy_bitmap(s->sprite);
    destroi_arma(s->arma);
    if (s->shielder1) destroi_shielder(s->shielder1);
    if (s->shielder2) destroi_shielder(s->shielder2);
    free(s);
    return NULL;
}


ball* cria_bola(short x, short y, ball* prox) {
    ball* novo = malloc(sizeof(ball));
    novo->x = x;
    novo->y = y;
    novo->raio = 0.0;
    novo->step = 4;
    novo->prox = (struct ball*)prox;

    return novo;
}

baller* cria_baller(short x, short y) {
    baller* novo = malloc(sizeof(baller));
    novo->sprite = al_load_bitmap("chefes/baller.png");
    novo->altura = 63;
    novo->largura = 87;
    novo->x = x;
    novo->y = y;
    novo->hp = 50;
    novo->step = 6;
    novo->destino = 0;
    novo->wait_time = 0;
    novo->andando = 0;

    novo->cooldown = 60;
    novo->arma = cria_arma(75);
    novo->bolas = NULL;

    return novo;
}

ball* move_bolas(ball* b, int x_max, int y_max) {
    if (!b) return NULL;

    if (b->raio < 15) {
        b->raio += 0.6;
    }
    b->x -= b->step;
    if (b->y + b->raio < 0) {
        ball* aux = b;
        b = move_bolas((ball*)b->prox, x_max, y_max);
        destroi_bola(aux);
    }
    else b->prox = (struct ball*)move_bolas((ball*)b->prox, x_max, y_max);
    return b;
}

void move_baller (baller* baller, int x_max, int y_max) {
    if (baller->x + baller->largura/2 > x_max - 10) {
        baller->x -= baller->step;
        return;
    }

    if (!baller->destino) baller->destino = rng(baller->altura, y_max - baller->altura);

    if (baller->cooldown == 0) {
            baller->bolas = cria_bola(baller->x - baller->largura/2, baller->y, baller->bolas);
            baller->cooldown = 45;
    }
    else baller->cooldown--;
    if (baller->arma->cooldown == 0) {
        atira(baller->arma, baller->x - baller->largura/2, baller->y - baller->altura/2 + 5, 6, 3);
        atira(baller->arma, baller->x - baller->largura/2, baller->y + baller->altura/2 - 5, 6, 3);
    }
    else baller->arma->cooldown--;

    baller->bolas = move_bolas(baller->bolas, x_max, y_max);
    baller->arma->disparos = move_balas(baller->arma->disparos, x_max, y_max);

    bala* ultd = baller->arma->disparos;
    ball* ultb = baller->bolas;
    unsigned char hit = 0;
    for (bala* d = baller->arma->disparos; d != NULL;) {
        ultb = baller->bolas;
        hit = 0;
        for (ball* b = baller->bolas; b != NULL;) {
            if (((b->x - d->x)*(b->x - d->x) + (b->y - d->y)*(b->y - d->y)) < b->raio + 4) {
                unsigned char cd = baller->arma->cooldown;
                atira(baller->arma, b->x, b->y, 6, 0);
                atira(baller->arma, b->x, b->y, 6, 1);
                atira(baller->arma, b->x, b->y, 6, 2);
                atira(baller->arma, b->x, b->y, 6, 3);
                baller->arma->cooldown = cd;
                if (ultd != d) {
					ultd->prox = d->prox;
					destroi_bala(d);
                    d = (bala*)ultd->prox;
				}
				else {
					baller->arma->disparos = (bala*)d->prox;
					destroi_bala(d);
                    ultd = d = baller->arma->disparos;
				}

                if (ultb != b) {
					ultb->prox = b->prox;
					destroi_bola(b);
                    b = (ball*)ultb->prox;
				}
				else {
					baller->bolas = (ball*)b->prox;
					destroi_bola(b);
                    ultb = b = baller->bolas;
				}
                hit = 1;
                break;
            }
            ultb = b;
            b = (ball*)b->prox;
        }
        if (hit) continue;
        ultd = d;
        d = (bala*)d->prox;
    }

    if (baller->andando) {
        if (baller->destino > baller->y ) baller->y += baller->step;
        else baller->y -= baller->step;   

        if (abs(baller->destino - baller->y) > baller->step) return;
        baller->y = baller->destino;
        baller->wait_time = 60;
        baller->andando = 0;
        return;
    }

    if (!baller->wait_time) {
        baller->andando = 1;
        baller->destino = rng(baller->altura, y_max - baller->altura);
    }
    else baller->wait_time--;
}

ball* destroi_bola(ball* b) {
    free(b);
    return NULL;
}