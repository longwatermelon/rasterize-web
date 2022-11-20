#include "light.h"
#include <stdlib.h>


struct Light *light_alloc(Vec3f pos, float in)
{
    struct Light *l = malloc(sizeof(struct Light));
    l->pos = pos;
    l->in = in;

    return l;
}


void light_free(struct Light *l)
{
    free(l);
}

