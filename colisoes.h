#ifndef __COLISOES__
#define __COLISOES__

#include <math.h>
#include "player.h"
#include "inimigos.h"
#include "chefes.h"
#include "drops.h"


int check_collision(player* p, inimigo* i, drop** d, spawner* s, baller* b);
inimigo* check_hit_on_enemy(bala* b, inimigo* i, bala** balas_perdidas, drop** drops, unsigned char* hit);
int check_hit_on_spawner(bala* b, spawner* s, bala** balas_perdidas);
int check_hit_on_baller(bala* d, baller* b, bala** balas_perdidas);
int check_hits(player* p, inimigo** i, spawner* s, baller* baller, bala** balas_perdidas, drop** drops);

#endif