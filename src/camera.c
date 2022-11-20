#include "camera.h"
#include <stdlib.h>


struct Camera *cam_alloc(Vec3f pos, Vec3f angle)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    c->pos = pos;
    c->angle = angle;
    c->pos_eff = (Vec3f){ 0, 0, 0 };
    c->angle_eff = (Vec3f){ 0, 0, 0 };

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}


void cam_apply_eff(struct Camera *c)
{
    c->pos = vec_addv(c->pos, c->pos_eff);
    c->angle = vec_addv(c->angle, c->angle_eff);
}


void cam_reverse_eff(struct Camera *c)
{
    c->pos = vec_sub(c->pos, c->pos_eff);
    c->angle = vec_sub(c->angle, c->angle_eff);
}

