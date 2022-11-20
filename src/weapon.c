#include "weapon.h"
#include "render.h"


struct Weapon *weapon_alloc(const char *fp, Vec3f pos, Vec3f angle, float divisor)
{
    struct Weapon *w = malloc(sizeof(struct Weapon));
    w->mesh = mesh_alloc((Vec3f){ 0.f, 0.f, 0.f }, (Vec3f){ 0.f, 0.f, 0.f }, fp, (SDL_Color){ 255, 200, 180 });
    w->default_pos = pos;
    w->pos = pos;
    w->angle = angle;
    w->absolute = false;

    w->divisor = divisor;

    return w;
}


void weapon_free(struct Weapon *w)
{
    mesh_free(w->mesh);
    free(w);
}


void weapon_move(struct Weapon *w, struct Camera *c)
{
    Vec3f pos = weapon_coords(w, w->pos, c);
    w->mesh->pos = vec_addv(w->mesh->pos, vec_divf(vec_sub(pos, w->mesh->pos), w->divisor));

    if (!w->absolute)
        mesh_rotate(w->mesh, vec_divf(vec_sub(vec_addv(c->angle, w->angle), w->mesh->rot), w->divisor));
//        w->mesh->rot = vec_addv(w->mesh->rot, vec_divf(vec_sub(vec_addv(c->angle, w->angle), w->mesh->rot), w->divisor));
}


void weapon_render(struct Weapon *w, RenderInfo *ri, struct Camera *c)
{
    mesh_render(w->mesh, ri, c);
}


Vec3f weapon_coords(struct Weapon *w, Vec3f c, struct Camera *cam)
{
    if (w->absolute)
        return c;
    else
        return vec_addv(cam->pos, render_rotate_cc(c, cam->angle));
}

