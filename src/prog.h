#ifndef PROG_H
#define PROG_H

#include "player.h"
#include "mesh.h"
#include "enemy.h"
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct Prog
{
    bool running;
    bool restart;

    SDL_Window *window;
    RenderInfo ri;

    SDL_Texture *scrtex;

    bool focused;

    struct Player *player;

    struct Mesh **solids;
    size_t nsolids;

    struct Enemy **enemies;
    size_t nenemies;

    int score;

    Uint32 shake_begin;
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);
void prog_events(struct Prog *p, SDL_Event *evt);
void prog_events_base(struct Prog *p, SDL_Event *evt);
void prog_events_game(struct Prog *p, SDL_Event *evt);
void prog_events_keystate(struct Prog *p);

void prog_mouse(struct Prog *p);
void prog_enemies(struct Prog *p);
void prog_player(struct Prog *p);
void prog_render(struct Prog *p);

bool prog_player_shoot(struct Prog *p, struct Enemy **e);

void prog_reset_buffers(struct Prog *p);

#endif

