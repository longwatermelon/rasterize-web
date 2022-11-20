#ifndef ENEMY_H
#define ENEMY_H

#include "util.h"
#include "mesh.h"
#include <time.h>

enum { ENEMY_NORMAL, ENEMY_DODGE, ENEMY_THROW };

struct Enemy
{
    int type;

    Vec3f pos;
    struct Mesh **body;
    size_t nbody;

    int health;
    bool dead;

    Uint32 dead_time;
    Vec3f *dead_animations;

    SDL_Color default_col;

    float dodge_shrink;
    int dodge_shrink_sign;

    struct Mesh *thrown[4];
    Vec3f thrown_vectors[4];
};

struct Enemy *enemy_alloc(Vec3f pos, int type);
void enemy_free(struct Enemy *e);

void enemy_render(struct Enemy *e, RenderInfo *ri, struct Camera *c);
void enemy_move(struct Enemy *e, SDL_Renderer *rend, Vec3f v);

bool enemy_ray_intersect(struct Enemy *e, Vec3f o, Vec3f dir, float *t);

int enemy_hurt(struct Enemy *e, int damage);

#endif

