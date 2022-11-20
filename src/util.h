#ifndef UTIL_H
#define UTIL_H

#include "vector.h"
#include "light.h"
#include <time.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    SDL_Renderer *rend;
    TTF_Font *font;
    struct Light **lights;
    size_t nlights;
    uint32_t scr[800 * 800];
    float zbuf[800 * 800];
} RenderInfo;

void ri_free(RenderInfo *ri);

Vec3f util_matmul(float mat[3][3], Vec3f p);
// Optimized for rotation matrices
Vec3f util_matmul_rotation_opt(float mat[3][3], Vec3f p);

float util_tri_area(Vec3f points[3]);

Vec3f util_barycentric_coefficients(Vec3f points[3], Vec3f p);

float util_p_line_sdist(Vec3f p, Vec3f p0, Vec3f p1);

#endif

