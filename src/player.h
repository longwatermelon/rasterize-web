#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"
#include "mesh.h"
#include "weapon.h"
#include "light.h"
#include <time.h>
#include <sys/types.h>
#include <SDL2/SDL_ttf.h>

struct Player
{
    struct Camera *cam;
    Vec3f vel;

    struct Weapon *weapon;

    struct Weapon *gun, *knife;
    bool scoped;

    bool knife_thrown;
    Vec3f knife_throw_origin;
    Uint32 knife_thrown_time;

    int health;

    Uint32 last_hurt;

    struct Light *light, *gun_light;
};

struct Player *player_alloc();
void player_free(struct Player *p);

void player_move(struct Player *p, struct Mesh **solids, size_t nsolids);
bool player_move_dir(struct Player *p, Vec3f dir, struct Mesh **solids, size_t nsolids, float bound);
bool player_check_dir(struct Player *p, Vec3f dir, struct Mesh **solids, size_t nsolids, float bound, float *min);

void player_render(struct Player *p, RenderInfo *ri);
void player_animate_weapon(struct Player *p);

void player_hurt(struct Player *p, int damage);

void player_switch_weapon(struct Player *p, struct Weapon *weapon);

#endif

