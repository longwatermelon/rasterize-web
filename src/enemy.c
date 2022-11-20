#include "enemy.h"
/* #include "audio.h" */
#include "render.h"


struct Enemy *enemy_alloc(Vec3f pos, int type)
{
    struct Enemy *e = malloc(sizeof(struct Enemy));
    e->type = type;
    e->pos = pos;

    e->dodge_shrink = 0.f;
    e->dodge_shrink_sign = 1;

    switch (type)
    {
    case ENEMY_NORMAL:
        e->nbody = 2;
        e->body = malloc(sizeof(struct Mesh*) * e->nbody);
        e->default_col = (SDL_Color){ 230, 150, 245 };
        e->body[0] = mesh_alloc(e->pos, (Vec3f){ 1.f, 1.f, .8f }, "res/donut.obj", e->default_col);
        e->body[1] = mesh_alloc(e->pos, (Vec3f){ .6f, 1.6f, 1.f }, "res/donut.obj", e->default_col);

        e->health = 5;
        break;
    case ENEMY_DODGE:
        e->nbody = 1;
        e->body = malloc(sizeof(struct Mesh*) * e->nbody);
        e->default_col = (SDL_Color){ 255, 255, 110 };
        e->body[0] = mesh_alloc(e->pos, (Vec3f){ 0.f, 0.f, 0.f }, "res/donut.obj", e->default_col);

        e->health = 3;
        break;

    case ENEMY_THROW:
        e->nbody = 4;
        e->body = malloc(sizeof(struct Mesh*) * e->nbody);
        e->default_col = (SDL_Color){ 139, 255, 110 };
        
        for (size_t i = 0; i < e->nbody; ++i)
        {
            Vec3f pos = {
                rand() % 4 - 2,
                rand() % 4 - 2,
                rand() % 4 - 2
            };

            e->body[i] = mesh_alloc(vec_addv(e->pos, pos), pos, "res/monkey.obj", e->default_col);
        }

        e->health = 30;
        break;
    }

    memset(e->thrown, 0, sizeof(struct Mesh*) * 5);

    e->dead_time = SDL_GetTicks();
    e->dead = false;

    e->dead_animations = malloc(sizeof(Vec3f) * e->nbody);

    for (size_t i = 0; i < e->nbody; ++i)
    {
        e->dead_animations[i] = (Vec3f){
            (float)(rand() % 100 - 50) / 100.f,
            (float)(rand() % 100 - 50) / 100.f,
            (float)(rand() % 100 - 50) / 100.f
        };
    }

    return e;
}


void enemy_free(struct Enemy *e)
{
    for (size_t i = 0; i < e->nbody; ++i)
        mesh_free(e->body[i]);

    free(e->dead_animations);

    free(e->body);
    free(e);
}


void enemy_render(struct Enemy *e, RenderInfo *ri, struct Camera *c)
{
    for (size_t i = 0; i < e->nbody; ++i)
    {
        Vec3f col = {
            e->body[i]->col.r,
            e->body[i]->col.g,
            e->body[i]->col.b
        };

        if (e->dead)
        {
            col = vec_addv(col, vec_divf(vec_sub((Vec3f){ 0, 0, 0 }, col), 50.f));
            e->body[i]->pos = vec_addv(e->body[i]->pos, e->dead_animations[i]);
            e->dead_animations[i].y += .0098f;
        }
        else
        {
            Vec3f def = { e->default_col.r, e->default_col.g, e->default_col.b };
            col = vec_addv(col, vec_divf(vec_sub(def, col), 10.f));
        }

        e->body[i]->col = (SDL_Color){ col.x, col.y, col.z };
    }

    if (e->type == ENEMY_NORMAL)
    {
        mesh_rotate(e->body[0], (Vec3f){ .06f, .04f, .07f });
        mesh_rotate(e->body[1], (Vec3f){ -.09f, .08f, -.04f });
    }
    else if (e->type == ENEMY_DODGE)
    {
        if (e->dodge_shrink >= .35f)
            e->dodge_shrink_sign = -1;
        if (e->dodge_shrink <= -.35f)
            e->dodge_shrink_sign = 1;

        e->dodge_shrink += e->dodge_shrink_sign * .01f;

        for (size_t i = 0; i < e->nbody; ++i)
        {
            mesh_rotate(e->body[i], (Vec3f){ .05f, -.1f, .02f });

            for (size_t j = 0; j < e->body[i]->npts; ++j)
            {
                Vec3f p = e->body[i]->pts[j];
                e->body[i]->pts[j] = vec_addv(p, vec_mulf(vec_sub(e->body[i]->pos, p), e->dodge_shrink / 100.f));
            }
        }
    }
    else if (e->type == ENEMY_THROW)
    {
        for (size_t i = 0; i < e->nbody; ++i)
        {
            mesh_rotate(e->body[i], (Vec3f){ .05f, .2f, .01f });
        }
    }

    for (size_t i = 0; i < e->nbody; ++i)
        mesh_render(e->body[i], ri, c);
}


void enemy_move(struct Enemy *e, SDL_Renderer *rend, Vec3f v)
{
    e->pos = vec_addv(e->pos, v);

    for (size_t i = 0; i < e->nbody; ++i)
    {
        if (e->body[i] == e->thrown[i])
        {
            e->body[i]->pos = vec_addv(e->body[i]->pos, e->thrown_vectors[i]);
            if (vec_len(vec_sub(e->body[i]->pos, e->pos)) > 50.f)
            {
                e->thrown[i] = 0;
                e->body[i]->pos = vec_addv(e->pos, (Vec3f){
                    rand() % 4 - 2,
                    rand() % 4 - 2,
                    rand() % 4 - 2
                });
            }
        }
        else
            e->body[i]->pos = vec_addv(e->body[i]->pos, v);
    }
}


bool enemy_ray_intersect(struct Enemy *e, Vec3f o, Vec3f dir, float *t)
{
    if (e->dead)
        return false;

    float min = INFINITY;

    for (size_t i = 0; i < e->nbody; ++i)
    {
        if (e->thrown[i])
            continue;

        float tmp;
        Triangle tri;

        if (mesh_ray_intersect(e->body[i], o, dir, &tmp, &tri))
        {
            if (tmp < min)
                min = tmp;
        }
    }

    if (t)
        *t = min;

    return min != INFINITY;
}


int enemy_hurt(struct Enemy *e, int damage)
{
    /* audio_play_sound("res/sfx/damage.wav"); */
    e->health -= damage;
    SDL_Color red = { 255, 0, 0 };

    for (size_t i = 0; i < e->nbody; ++i)
    {
        if (!e->thrown[i])
            e->body[i]->col = red;
    }

    if (e->health <= 0)
    {
        /* audio_play_sound("res/sfx/explode.wav"); */
        e->dead = true;
        e->dead_time = SDL_GetTicks();
        
        switch (e->type)
        {
        case ENEMY_NORMAL: return 1;
        case ENEMY_DODGE: return 5;
        case ENEMY_THROW: return 20;
        }
    }

    return 0;
}

