#ifndef MESH_H
#define MESH_H

#include "util.h"
#include "camera.h"
#include "light.h"
#include <SDL2/SDL.h>
#include <sys/types.h>

typedef struct
{
    int idx[3];
    int nidx;
} Triangle;

struct Mesh
{
    Vec3f pos;
    Vec3f rot;

    Vec3f *opts;
    Vec3f *pts;
    size_t npts;

    Triangle *tris;
    size_t ntris;

    Vec3f *onorms;
    Vec3f *norms;
    size_t nnorms;

    SDL_Color col;
    bool bculling;
};

struct Mesh *mesh_alloc(Vec3f pos, Vec3f rot, const char *fp, SDL_Color col);
void mesh_free(struct Mesh *m);

void mesh_read(struct Mesh *m, const char *fp);

void mesh_render(struct Mesh *m, RenderInfo *ri, struct Camera *c);

void mesh_rotate(struct Mesh *m, Vec3f angle);

bool mesh_ray_intersect(struct Mesh *m, Vec3f ro, Vec3f rdir, float *t, Triangle *tri);
bool mesh_ray_tri_intersect(struct Mesh *m, Triangle tri, Vec3f ro, Vec3f rdir, float *t);

float mesh_point_shortest_dist(struct Mesh *m, Vec3f p);
float mesh_point_shortest_dist_tri(struct Mesh *m, Triangle tri, Vec3f p);

#endif

