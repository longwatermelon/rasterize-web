#ifndef WEAPON_H
#define WEAPON_H

#include "mesh.h"
#include "camera.h"

struct Weapon
{
    struct Mesh *mesh;
    Vec3f pos, angle;
    Vec3f default_pos;
    float divisor;
    bool absolute;
};

struct Weapon *weapon_alloc(const char *fp, Vec3f pos, Vec3f angle, float divisor);
void weapon_free(struct Weapon *w);

void weapon_move(struct Weapon *w, struct Camera *c);

void weapon_render(struct Weapon *w, RenderInfo *ri, struct Camera *c);

Vec3f weapon_coords(struct Weapon *w, Vec3f c, struct Camera *cam);

#endif

