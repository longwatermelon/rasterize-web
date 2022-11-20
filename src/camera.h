#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

struct Camera
{
    Vec3f pos, angle;
    Vec3f pos_eff, angle_eff;
};

struct Camera *cam_alloc(Vec3f pos, Vec3f angle);
void cam_free(struct Camera *c);

void cam_apply_eff(struct Camera *c);
void cam_reverse_eff(struct Camera *c);

#endif

