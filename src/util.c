#include "util.h"
#include <stdio.h>
#include <math.h>

void ri_free(RenderInfo *ri)
{
    TTF_CloseFont(ri->font);

    for (size_t i = 0; i < ri->nlights; ++i)
        light_free(ri->lights[i]);

    free(ri->lights);
}

Vec3f util_matmul(float mat[3][3], Vec3f p)
{
    Vec3f ret;

    ret.x = p.x * mat[0][0] + p.y * mat[1][0] + p.z * mat[2][0];
    ret.y = p.x * mat[0][1] + p.y * mat[1][1] + p.z * mat[2][1];
    ret.z = p.x * mat[0][2] + p.y * mat[1][2] + p.z * mat[2][2];

    return ret;
}


Vec3f util_matmul_rotation_opt(float mat[3][3], Vec3f p)
{
    if (mat[0][0] == 1 && mat[1][1] == 1 && mat[2][2] == 1)
        return p;

    return util_matmul(mat, p);
}


float util_tri_area(Vec3f points[3])
{
    float a = vec_len(vec_sub(points[1], points[0]));
    float b = vec_len(vec_sub(points[2], points[0]));
    float c = vec_len(vec_sub(points[2], points[1]));

    float s = (a + b + c) / 2.f;

    return sqrtf(s * (s - a) * (s - b) * (s - c));
}


Vec3f util_barycentric_coefficients(Vec3f points[3], Vec3f p)
{
    Vec3f ba = vec_sub(points[1], points[0]);
    Vec3f ca = vec_sub(points[2], points[0]);

    Vec3f pa = vec_sub(p, points[0]);
    Vec3f pb = vec_sub(p, points[1]);
    Vec3f pc = vec_sub(p, points[2]);

    Vec3f n = vec_cross(ba, ca);
    float s_abc = vec_len(n);
    n = vec_normalize(n);

    Vec3f pbc = vec_cross(pb, pc);
    float s_pbc = vec_dot(n, pbc);

    Vec3f apc = vec_cross(pa, ca);
    float s_apc = vec_dot(n, apc);

    Vec3f abp = vec_cross(ba, pa);
    float s_abp = vec_dot(n, abp);

    return (Vec3f){
        s_pbc / s_abc,
        s_apc / s_abc,
        s_abp / s_abc
    };
}


float util_p_line_sdist(Vec3f p, Vec3f p0, Vec3f p1)
{
    float len = vec_len(vec_sub(p1, p0));
    float t = vec_dot(vec_sub(p1, p0), vec_sub(p, p0)) / len;

    if (t < 0.f) return vec_len(vec_sub(p, p0));
    if (t > len) return vec_len(vec_sub(p, p1));

    float h = vec_len(vec_sub(p, p0));
    return sqrtf(h * h - t * t);
}

