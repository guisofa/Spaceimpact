#include "inimigos.h"

int rng(int min, int max) {
    int r = rand();
    return ((r % (max - min + 1)) + min);
}

inimigo* cria_inimigo(short x, short y, int x_max, int y_max, unsigned char tipo, inimigo* lista) {
    inimigo* novo = malloc(sizeof(inimigo));

    switch (tipo) {
        case 0:
            novo->sprite = al_load_bitmap("inimigos/0.png");
            novo->altura = 32;
            novo->largura = 64;
            novo->x = x;
            novo->y = y;
            novo->hp = 3;
            novo->step = 3;
            novo->arma = NULL;
            novo->tipo = tipo;
            novo->flags[0] = 0;
            novo->prox = (struct inimigo*)lista;
            break;
        case 1:
            novo->sprite = al_load_bitmap("inimigos/1.png");
            novo->altura = 24;
            novo->largura = 58;
            novo->x = x;
            novo->y = y;
            novo->hp = 2;
            novo->step = 8;
            novo->arma = cria_arma(30);
            novo->tipo = tipo;
            novo->flags[0] = (y <= y_max/2);
            novo->flags[1] = 0;
            novo->prox = (struct inimigo*)lista;
            break;
        case 2:
            novo->sprite = al_load_bitmap("inimigos/2.png");
            novo->altura = 32;
            novo->largura = 64;
            novo->x = x;
            novo->y = y;
            novo->hp = 2;
            novo->step = 5;
            novo->arma = cria_arma(45);
            novo->tipo = tipo;
            novo->flags[0] = 0;
            novo->flags[1] = 0;
            novo->prox = (struct inimigo*)lista;
            break;
        case 3:
            novo->sprite = al_load_bitmap("inimigos/3.png");
            novo->altura = 139;
            novo->largura = 125;
            novo->x = x;
            novo->y = y;
            novo->hp = 10;
            novo->step = 1;
            novo->arma = NULL;
            novo->tipo = tipo;
            novo->flags[0] = 0;
            novo->prox = (struct inimigo*)lista;
            break;
        case 4:
            novo->sprite = al_load_bitmap("inimigos/4.png");
            novo->altura = 31;
            novo->largura = 37;
            novo->x = x;
            novo->y = y;
            novo->hp = 1;
            novo->step = 3;
            novo->arma = cria_arma(25);
            novo->tipo = tipo;
            novo->flags[0] = 0;
            novo->flags[1] = (y <= y_max/2);
            novo->prox = (struct inimigo*)lista;
    }

    return novo;
}

inimigo* move_inimigo(inimigo* i, int x_max, int y_max) {
    if (!i) return NULL;

    inimigo* aux;
    switch(i->tipo) {
        case 3:
        case 0:
            i->x -= i->step;
            if (i->x + i->largura/2 < 0) {
                aux = i;
                i = move_inimigo((inimigo*)i->prox, x_max, y_max);
                destroi_inimigo(aux);
            }
            else i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
            return i;
        case 1:
            if (i->flags[1])
                i->x += i->step;
            else i->x -= i->step;

            if (i->x - i->largura/2 <= 0) i->flags[1] = 1;
            if (i->x + i->largura/2 >= x_max) i->flags[1] = 0;

            if (i->arma->cooldown == 0)
                if (i->flags[0])
                    atira(i->arma, i->x, i->y + i->altura/2, 5, 1);
                else atira(i->arma, i->x, i->y - i->altura/2, 5, 0);
            else i->arma->cooldown--;
           
            i->arma->disparos = move_balas(i->arma->disparos, x_max, y_max);

            i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
            return i;
        case 2:
            if (i->x > x_max - i->largura - 20) {
                i->x -= i->step;
                i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
                return i;
            }
            if (!i->flags[1]) i->flags[1] = rng(i->altura/2, y_max - i->altura/2);

            if (abs(i->flags[1] - i->y) > i->step) {
                if (i->flags[1] > i->y ) i->y += i->step;
                else i->y -= i->step;
                i->arma->disparos = move_balas(i->arma->disparos, x_max, y_max);
                i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
                return i;
            }

            i->y = i->flags[1];
            if (i->arma->cooldown == 0) {
                atira(i->arma, i->x - i->largura/2, i->y, 5, 3);
                i->flags[1] = rng(i->altura/2, y_max - i->altura/2);
            }
            else i->arma->cooldown--;
            i->arma->disparos = move_balas(i->arma->disparos, x_max, y_max);
            
            i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
            return i;
        case 4:
            i->x -= i->step;
            if (i->flags[1]) i->y += 2 * i->step;
            else i->y -= 2 * i->step;
            if (i->y <= 0) i->flags[1] = 1;
            if (i->y >= y_max) i->flags[1] = 0;
            
            if (i->x + i->largura/2 < 0) {
                aux = i;
                i = move_inimigo((inimigo*)i->prox, x_max, y_max);
                destroi_inimigo(aux);
            }
            else {
                if (i->arma->cooldown == 0) {
                    atira(i->arma, i->x - i->largura/2, i->y - ((1.0/4.0) * i->altura), 10, 3);
                    atira(i->arma, i->x - i->largura/2, i->y + ((1.0/4.0) * i->altura), 10, 3);
                }
                else i->arma->cooldown--;
                i->arma->disparos = move_balas(i->arma->disparos, x_max, y_max);
                i->prox = (struct inimigo*)move_inimigo((inimigo*)i->prox, x_max, y_max);
            }
            return i;
    }
    return NULL;
}

inimigo* destroi_inimigo(inimigo* i) {
    if (!i) return NULL;
    al_destroy_bitmap(i->sprite);
    if (i->arma) destroi_arma(i->arma);
    free(i);
    return NULL;
}