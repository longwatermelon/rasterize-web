#include "mesh.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct Mesh *mesh_alloc(Vec3f pos, Vec3f rot, const char *fp, SDL_Color col)
{
    struct Mesh *m = malloc(sizeof(struct Mesh));
    m->pos = pos;
    m->rot = rot;

    m->opts = 0;
    m->pts = 0;
    m->npts = 0;

    m->tris = 0;
    m->ntris = 0;

    m->onorms = 0;
    m->norms = 0;
    m->nnorms = 0;

    m->col = col;
    m->bculling = true;

    mesh_read(m, fp);

    m->opts = malloc(sizeof(Vec3f) * m->npts);
    memcpy(m->opts, m->pts, sizeof(Vec3f) * m->npts);

    m->onorms = malloc(sizeof(Vec3f) * m->nnorms);
    memcpy(m->onorms, m->norms, sizeof(Vec3f) * m->nnorms);

    mesh_rotate(m, rot);

    return m;
}


void mesh_free(struct Mesh *m)
{
    free(m->pts);
    free(m->tris);
    free(m->norms);

    free(m->onorms);
    free(m->opts);

    free(m);
}


void mesh_read(struct Mesh *m, const char *fp)
{
    FILE *f = fopen(fp, "r");

    if (!f)
    {
        fprintf(stderr, "Error: couldn't open file '%s'.\n", fp);
        exit(EXIT_FAILURE);
    }

    char *line = 0;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1)
    {
        if (line[0] == 'v')
        {
            if (line[1] == ' ')
            {
                m->pts = realloc(m->pts, sizeof(Vec3f) * ++m->npts);
                Vec3f *p = &m->pts[m->npts - 1];

                sscanf(line, "%*s %f %f %f", &p->x, &p->y, &p->z);
            }
            else if (line[1] == 'n')
            {
                m->norms = realloc(m->norms, sizeof(Vec3f) * ++m->nnorms);
                Vec3f *n = &m->norms[m->nnorms - 1];

                sscanf(line, "%*s %f %f %f", &n->x, &n->y, &n->z);
            }
        }
        else if (line[0] == 'f')
        {
            int ws = 0;

            for (int i = 0; i < strlen(line); ++i)
            {
                if (line[i] == ' ')
                    ++ws;
            }

            if (ws != 3)
            {
                fprintf(stderr, "Error in file '%s': faces must be triangulated.\n", fp);
                exit(EXIT_FAILURE);
            }

            m->tris = realloc(m->tris, sizeof(Triangle) * ++m->ntris);

            int idx[3];
            int norm;
            sscanf(line, "%*s %d/%*d/%d %d%*s %d%*s", idx, &norm, idx + 1, idx + 2);

            m->tris[m->ntris - 1].idx[0] = idx[0] - 1;
            m->tris[m->ntris - 1].idx[1] = idx[1] - 1;
            m->tris[m->ntris - 1].idx[2] = idx[2] - 1;

            m->tris[m->ntris - 1].nidx = norm - 1;
        }
    }

    free(line);
    fclose(f);
}


void mesh_render(struct Mesh *m, RenderInfo *ri, struct Camera *c)
{
    for (size_t i = 0; i < m->ntris; ++i)
    {
        Vec3f mpts[3] = {
            vec_addv(m->pts[m->tris[i].idx[0]], m->pos),
            vec_addv(m->pts[m->tris[i].idx[1]], m->pos),
            vec_addv(m->pts[m->tris[i].idx[2]], m->pos)
        };

        Vec3f norm = m->norms[m->tris[i].nidx];

        float tri_dist = vec_len(vec_sub(c->pos, mpts[0]));
        if (tri_dist > 100.f)
            continue;

        if (m->bculling)
        {
            Vec3f v = mpts[0];
            Vec3f vp = vec_normalize(vec_sub(v, c->pos));

            if (vec_dot(vp, norm) >= 0.f)
                continue;
        }

        SDL_Point points[3];
        bool render = true;

        float zvals[3];

        for (int j = 0; j < 3; ++j)
        {
            Vec3f p = mpts[j];
            p = vec_sub(p, c->pos);
            p = render_rotate_ccw(p, c->angle);
            zvals[j] = p.z;

            if (p.z < .5f)
            {
                render = false;
                break;
            }
            else
            {
                points[j] = render_project_point(p);
            }
        }

        if (render)
        {
            float dlight = 0.f, slight = 0.f;

            for (size_t i = 0; i < ri->nlights; ++i)
            {
                if (ri->lights[i]->in == 0.f)
                    continue;

                float b = fmin(ri->lights[i]->in / (.005f * tri_dist * tri_dist), ri->lights[i]->in);
                Vec3f l = vec_normalize(vec_sub(c->pos, mpts[0]));
                dlight += b * fmax(0.f, vec_dot(l, norm));

                if (dlight == 0.f)
                {
                    norm = vec_mulf(norm, -1.f);
                    dlight = b * fmax(0.f, vec_dot(l, norm));
                }

                Vec3f r = vec_sub(l, vec_mulf(vec_mulf(norm, 2.f), vec_dot(l, norm)));
                slight += b * powf(fmax(0.f, vec_dot(r, vec_normalize(mpts[0]))), 90.f);
            }

            if (dlight > .01f)
            {
                SDL_Color col = {
                    fmin(slight + dlight * m->col.r, 255),
                    fmin(slight + dlight * m->col.g, 255),
                    fmin(slight + dlight * m->col.b, 255)
                };
                render_filled_tri(points, zvals, ri->scr, ri->zbuf, col);
            }
        }
    }
}


void mesh_rotate(struct Mesh *m, Vec3f angle)
{
    m->rot = vec_addv(m->rot, angle);

    for (size_t i = 0; i < m->npts; ++i)
        m->pts[i] = render_rotate_cc(m->opts[i], m->rot);

    for (size_t i = 0; i < m->nnorms; ++i)
        m->norms[i] = render_rotate_cc(m->onorms[i], m->rot);
}


bool mesh_ray_intersect(struct Mesh *m, Vec3f ro, Vec3f rdir, float *t, Triangle *tri)
{
    float nearest = INFINITY;
    *t = INFINITY;

    for (size_t i = 0; i < m->ntris; ++i)
    {
        if (m->bculling && vec_dot(rdir, m->norms[m->tris[i].nidx]) > 0.f)
            continue;

        if (mesh_ray_tri_intersect(m, m->tris[i], ro, rdir, &nearest))
        {
            if (nearest < *t)
            {
                *t = nearest;
                *tri = m->tris[i];
            }
        }
    }

    return *t != INFINITY;
}


bool mesh_ray_tri_intersect(struct Mesh *m, Triangle tri, Vec3f ro, Vec3f rdir, float *t)
{
    // find intersection point
    Vec3f a = m->pts[tri.idx[0]];
    a = vec_addv(a, m->pos);

    Vec3f b = m->pts[tri.idx[1]];
    b = vec_addv(b, m->pos);

    Vec3f c = m->pts[tri.idx[2]];
    c = vec_addv(c, m->pos);

    Vec3f norm = m->norms[tri.nidx];
    *t = (vec_dot(a, norm) - vec_dot(ro, norm)) / vec_dot(rdir, norm);

    // check if inside triangle
    Vec3f p = vec_addv(ro, vec_mulf(rdir, *t));

    Vec3f ca = vec_sub(c, a);
    Vec3f ba = vec_sub(b, a);

    float a1 = vec_dot(ba, ba);
    float b1 = vec_dot(ca, ba);
    float b2 = vec_dot(ca, ca);

    float c1 = vec_dot(ba, vec_sub(p, a));
    float c2 = vec_dot(ca, vec_sub(p, a));

    float y = ((c1 * b1) - (c2 * a1)) / ((b1 * b1) - (a1 * b2));
    float x = (c1 - (b1 * y)) / a1;

    return (x >= 0.f && x <= 1.f && y >= 0.f && y <= 1.f && x + y >= 0.f && x + y <= 1.f && *t >= 0.f);
}


float mesh_point_shortest_dist(struct Mesh *m, Vec3f p)
{
    float t = INFINITY;

    for (size_t i = 0; i < m->ntris; ++i)
    {
        float dist = mesh_point_shortest_dist_tri(m, m->tris[i], p);

        if (dist < t)
            t = dist;
    }

    return t;
}


float mesh_point_shortest_dist_tri(struct Mesh *m, Triangle tri, Vec3f p)
{
    float t = INFINITY;

    Vec3f a = vec_addv(m->pos, m->pts[tri.idx[0]]);
    Vec3f b = vec_addv(m->pos, m->pts[tri.idx[1]]);
    Vec3f c = vec_addv(m->pos, m->pts[tri.idx[2]]);

    t = fabsf(vec_dot(vec_sub(a, p), m->norms[tri.nidx]));

    Vec3f points[3] = { a, b, c };

    Vec3f coefficients = util_barycentric_coefficients(points, vec_addv(p, vec_mulf(m->norms[tri.nidx], t)));

    int negatives = 0;
    if (coefficients.x < 0.f) ++negatives;
    if (coefficients.y < 0.f) ++negatives;
    if (coefficients.z < 0.f) ++negatives;

    if (negatives == 0)
    {
        return t;
    }
    else if (negatives == 1)
    {
        if (coefficients.x < 0.f) return util_p_line_sdist(p, points[1], points[2]);
        if (coefficients.y < 0.f) return util_p_line_sdist(p, points[0], points[2]);
        if (coefficients.z < 0.f) return util_p_line_sdist(p, points[0], points[1]);
    }
    else
    {
        if (coefficients.x >= 0.f) return vec_len(vec_sub(points[0], p));
        if (coefficients.y >= 0.f) return vec_len(vec_sub(points[1], p));
        if (coefficients.z >= 0.f) return vec_len(vec_sub(points[2], p));
    }

    return t;
}

