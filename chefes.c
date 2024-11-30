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
            if (abs(shielder->y - (spawner->y - spawner->altura/2 - 50)) < shielder->step) {
                shielder->y = spawner->y - spawner->altura/2 - 50;
                shielder->linked = 1;
            }
        } else {
            shielder->y += shielder->step;
            if (abs(shielder->y - (spawner->y + spawner->altura/2 + 50)) < shielder->step) {
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
    al_destroy_bitmap(s->sprite);
    destroi_arma(s->arma);
    return NULL;
}