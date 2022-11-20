#include "vector.h"
#include <math.h>
#include <stdio.h>


Vec3f vec_normalize(Vec3f v)
{
    float len = vec_len(v);

    if (len == 0.f)
        return (Vec3f){ 0.f, 0.f, 0.f };

    return (Vec3f){
        .x = v.x / len,
        .y = v.y / len,
        .z = v.z / len
    };
}


float vec_len(Vec3f v)
{
    return sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );
}


Vec3f vec_addv(Vec3f v1, Vec3f v2)
{
    return (Vec3f){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z
    };
}


Vec3f vec_addf(Vec3f v, float f)
{
    return (Vec3f){
        .x = v.x + f,
        .y = v.y + f,
        .z = v.z + f
    };
}


Vec3f vec_sub(Vec3f v1, Vec3f v2)
{
    return (Vec3f){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z
    };
}


Vec3f vec_neg(Vec3f v)
{
    return (Vec3f){
        .x = -v.x,
        .y = -v.y,
        .z = -v.z
    };
}


float vec_dot(Vec3f v1, Vec3f v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


Vec3f vec_mulf(Vec3f v, float f)
{
    return (Vec3f){
        .x = v.x * f,
        .y = v.y * f,
        .z = v.z * f
    };
}


Vec3f vec_divf(Vec3f v, float f)
{
    return (Vec3f){
        .x = v.x / f,
        .y = v.y / f,
        .z = v.z / f
    };
}


Vec3f vec_cross(Vec3f v1, Vec3f v2)
{
    Vec3f cross;
    cross.x = v1.y * v2.z - v1.z * v2.y;
    cross.y = v1.z * v2.x - v1.x * v2.z;
    cross.z = v1.x * v2.y - v1.y * v2.x;

    return cross;
}


bool vec_cmp(Vec3f v1, Vec3f v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}


void vec_print(Vec3f v)
{
    printf("x: %f | y: %f | z: %f\n", v.x, v.y, v.z);
}

