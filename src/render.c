#include "render.h"

#define swapp(x, y) { \
    SDL_Point tmp = x; \
    x = y; \
    y = tmp; }

#define swapf(x, y) { \
    float tmp = x; \
    x = y; \
    y = tmp; }

RTI g_r01, g_r02, g_r12;

SDL_Point render_project_point(Vec3f p)
{
    float px = 0.f, py = 0.f;

    if (p.z != 0.f)
    {
        px = p.x / p.z;
        py = p.y / p.z;
    }

    SDL_Point proj = {
        (px + .5f) * 800.f,
        (py + .5f) * 800.f
    };

    return proj;
}


Vec3f render_rotate_ccw(Vec3f p, Vec3f angle)
{
    if (vec_len(angle) == 0.f) return p;

    float rotx[3][3] = {
        { 1, 0, 0 },
        { 0, cosf(angle.y), -sinf(angle.y) },
        { 0, -rotx[1][2], rotx[1][1] }
    };

    float roty[3][3] = {
        { cosf(angle.x), 0, sinf(angle.x) },
        { 0, 1, 0 },
        { -roty[0][2], 0, roty[0][0] }
    };

    float rotz[3][3] = {
        { cosf(angle.z), -sinf(angle.z), 0 },
        { -rotz[0][1], rotz[0][0], 0 },
        { 0, 0, 1 }
    };

    return util_matmul_rotation_opt(rotz, util_matmul_rotation_opt(rotx, util_matmul_rotation_opt(roty, p)));
}


Vec3f render_rotate_cc(Vec3f p, Vec3f angle)
{
    if (vec_len(angle) == 0.f) return p;
    angle = vec_mulf(angle, -1.f);

    float rotx[3][3] = {
        { 1, 0, 0 },
        { 0, cosf(angle.y), -sinf(angle.y) },
        { 0, -rotx[1][2], rotx[1][1] }
    };

    float roty[3][3] = {
        { cosf(angle.x), 0, sinf(angle.x) },
        { 0, 1, 0 },
        { -roty[0][2], 0, roty[0][0] }
    };

    float rotz[3][3] = {
        { cosf(angle.z), -sinf(angle.z), 0 },
        { -rotz[0][1], rotz[0][0], 0 },
        { 0, 0, 1 }
    };

    return util_matmul_rotation_opt(roty, util_matmul_rotation_opt(rotx, util_matmul_rotation_opt(rotz, p)));
}


SDL_Texture *render_text(SDL_Renderer *rend, TTF_Font *font, const char *s)
{
    if (strlen(s) == 0)
        return 0;

    SDL_Surface *surf = TTF_RenderText_Blended(font, s, (SDL_Color){ 255, 255, 255 });
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);

    SDL_FreeSurface(surf);
    return tex;
}


void render_filled_tri(SDL_Point p[3], float z[3], uint32_t *screen, float *zbuf, SDL_Color col)
{
    SDL_Point p0 = p[0], p1 = p[1], p2 = p[2];
    float z0 = z[0], z1 = z[1], z2 = z[2];

    if (p0.y > p1.y)
    {
        swapp(p0, p1);
        swapf(z0, z1);
    }

    if (p0.y > p2.y)
    {
        swapp(p0, p2);
        swapf(z0, z2);
    }

    if (p1.y > p2.y)
    {
        swapp(p1, p2);
        swapf(z1, z2);
    }

    g_r02 = (RTI){ .x = p0.x, .z = z0, .sx = (float)(p2.y - p0.y) / (p2.x - p0.x), .sz = (float)(z2 - z0) / (p2.y - p0.y) };
    g_r01 = (RTI){ .x = p0.x, .z = z0, .sx = (float)(p1.y - p0.y) / (p1.x - p0.x), .sz = (float)(z1 - z0) / (p1.y - p0.y) };
    g_r12 = (RTI){ .x = p1.x, .z = z1, .sx = (float)(p2.y - p1.y) / (p2.x - p1.x), .sz = (float)(z2 - z1) / (p2.y - p1.y) };

    render_fill_edges(p0.y, p1.y, &g_r02, &g_r01, screen, zbuf, col);
    render_fill_edges(p1.y, p2.y, &g_r02, &g_r12, screen, zbuf, col);
}


void render_fill_edges(int top, int bot, RTI *l1, RTI *l2, uint32_t *screen, float *zbuf, SDL_Color col)
{
    uint32_t hex_col = 0x00000000 | col.r << 16 | col.g << 8 | col.b;

    for (int y = top; y < bot; ++y)
    {
        int min = roundf(l1->x > l2->x ? l2->x : l1->x);
        int max = roundf(l1->x > l2->x ? l1->x : l2->x);

        float z = l1->z;
        float sz = (l2->z - l1->z) / (l2->x - l1->x);

        for (int i = min; i < max; ++i)
        {
            z += sz;

            if (i < 0)
            {
                z += sz * -i;
                i = 0;
                continue;
            }

            if (i >= 800) break;

            int idx = y * 800 + i;

            if (idx >= 0 && idx < 800 * 800)
            {
                if (z < zbuf[idx])
                {
                    screen[idx] = hex_col;
                    zbuf[idx] = z;
                }
            }
        }

        l1->x += 1.f / l1->sx;
        l2->x += 1.f / l2->sx;

        l1->z += l1->sz;
        l2->z += l2->sz;
    }
}

