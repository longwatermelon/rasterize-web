#include "prog.h"
/* #include "audio.h" */
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
/* #include <SDL2/SDL_mixer.h> */
#include <emscripten/emscripten.h>

struct Prog *p;

void emloop(void *arg)
{
    prog_mainloop(p);

    if (!p->running && p->restart)
    {
        SDL_Window *w = p->window;
        SDL_Renderer *r = p->ri.rend;
        prog_free(p);
        p = prog_alloc(w, r);
    }
}

int main(int argc, char **argv)
{
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    /* Mix_Init(MIX_INIT_MOD); */

    /* audio_init(); */

    SDL_Window *w = SDL_CreateWindow("Rasterize", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    p = prog_alloc(w, r);
    emscripten_set_main_loop_arg(emloop, p, -1, 1);
    /* audio_cleanup(); */

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    
    /* Mix_Quit(); */
    TTF_Quit();
    SDL_Quit();

    return 0;
}

