#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct
{
    float x, y, z;
} Vec3f;

Vec3f vec_normalize(Vec3f v);
float vec_len(Vec3f v);

Vec3f vec_addv(Vec3f v1, Vec3f v2);
Vec3f vec_addf(Vec3f v, float f);

Vec3f vec_sub(Vec3f v1, Vec3f v2);
Vec3f vec_neg(Vec3f v);

float vec_dot(Vec3f v1, Vec3f v2);
Vec3f vec_mulf(Vec3f v, float f);

Vec3f vec_divf(Vec3f v, float f);

Vec3f vec_cross(Vec3f v1, Vec3f v2);

bool vec_cmp(Vec3f v1, Vec3f v2);

void vec_print(Vec3f v);

#endif

