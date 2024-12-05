#include "drops.h"

drop* cria_drop (short x, short y, unsigned char tag, drop* prox) {
    drop* novo = malloc(sizeof(drop));

    if (tag == 0) novo->sprite = al_load_bitmap("drops/0.png");
    else if (tag == 1) novo->sprite = al_load_bitmap("drops/1.png");
    else if (tag == 2) novo->sprite = al_load_bitmap("drops/2.png");
    else if (tag == 3) novo->sprite = al_load_bitmap("drops/3.png");
    
    novo->x = x;
    novo->y = y;
    novo->tag = tag;
    novo->prox = (struct drop*)prox;
    novo->ante = NULL;

    if (prox)
        prox->ante = (struct drop*)novo;

    return novo;
}

drop* move_drop (drop* d, int x_max, int y_max) {
    if (!d) return NULL;

    d->x -= DROP_STEP;
    if (d->x + DROP_TAM/2 < 0) {
        drop* aux = d;
        d = move_drop((drop*)d->prox, x_max, y_max);
        if (d)
            d->ante = aux->ante;
        destroi_drop(aux);
    }
    else d->prox = (struct drop*)move_drop((drop*)d->prox, x_max, y_max);
    
    return d;
}

drop* destroi_drop(drop* d) {
    al_destroy_bitmap(d->sprite);
    free(d);
    return NULL;
}