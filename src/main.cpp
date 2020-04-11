#include "Config.hpp"
#include "Options.hpp"
#include "World.hpp"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace anosmellya;

static bool is_quit_event(SDL_Event& event)
{
    return event.type == SDL_QUIT
        || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_q);
}

static void lock(SDL_mutex* mtx, const char* role)
{
    if (SDL_LockMutex(mtx)) {
        fprintf(stderr, "%s failed to lock mutex; %s\n", role, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

struct SimThreadShared {
    World world;
    SDL_mutex* world_mtx;
    SDL_sem* tick_sem;
    bool done;
};

static int sim_thread_loop(void* arg)
{
    SimThreadShared* shared = (SimThreadShared*)arg;
    for (;;) {
        SDL_SemWait(shared->tick_sem);
        lock(shared->world_mtx, "Child");
        if (shared->done) {
            SDL_UnlockMutex(shared->world_mtx);
            return 0;
        }
        shared->world.simulate();
        SDL_UnlockMutex(shared->world_mtx);
    }
}

static void simulate(SDL_Renderer* renderer, Options const& opts)
{
    SDL_Event event;
    Random random(opts.seed);
    SDL_Thread* sim_thread;
    SimThreadShared shared
        = { World(opts.world_width, opts.world_height, random, opts.conf),
              SDL_CreateMutex(), SDL_CreateSemaphore(0), false };
    if (!shared.world_mtx || !shared.tick_sem
        || !(sim_thread = SDL_CreateThread(
                 sim_thread_loop, "AnosmellyaSimThread", (void*)&shared))) {
        fprintf(
            stderr, "Failed to allocate thread stuff; %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Statistics stats;
    World my_world;
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        lock(shared.world_mtx, "Parent");
        my_world.copy(shared.world);
        SDL_UnlockMutex(shared.world_mtx);
        SDL_SemPost(shared.tick_sem);
        while (SDL_PollEvent(&event)) {
            if (is_quit_event(event)) {
                lock(shared.world_mtx, "Parent");
                shared.done = true;
                SDL_UnlockMutex(shared.world_mtx);
		SDL_SemPost(shared.tick_sem);
                SDL_WaitThread(sim_thread, NULL);
                SDL_DestroyMutex(shared.world_mtx);
                SDL_DestroySemaphore(shared.tick_sem);
                return;
            }
        }
        if (opts.print_stats) {
            my_world.get_statistics(stats);
            stats.print(stdout);
            putchar('\n');
            fflush(stdout);
        }
        if (opts.draw) {
            my_world.draw(renderer);
            SDL_RenderPresent(renderer);
        }
        Uint32 new_ticks = SDL_GetTicks();
        if (new_ticks - ticks < opts.frame_delay) {
            SDL_Delay(opts.frame_delay - (new_ticks - ticks));
        }
    }
}

int main(int argc, char* argv[])
{
    Options opts(argc, argv);
    int status = EXIT_FAILURE;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Uint32 init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS;
    if (opts.draw) {
        init_flags |= SDL_INIT_VIDEO;
    }
    if (SDL_Init(init_flags)) {
        fprintf(stderr, "SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    if (opts.draw) {
        // Metal is amazingly slow for some reason; try using OpenGL instead:
        SDL_RendererInfo renderer_info;
        if (SDL_GetRenderDriverInfo(0, &renderer_info) == 0
            && !strcmp(renderer_info.name, "metal")) {
            SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        }
        window = SDL_CreateWindow("Anosmellya", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, opts.world_width * opts.pixel_size,
            opts.world_height * opts.pixel_size,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!window) {
            fprintf(stderr, "SDL window creation failed; %s\n", SDL_GetError());
            goto error_create_window;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            fprintf(
                stderr, "SDL renderer creation failed; %s\n", SDL_GetError());
            goto error_create_surface;
        }
        // Just in case the background is another color:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    simulate(renderer, opts);
    status = EXIT_SUCCESS;
    if (opts.draw) {
        SDL_DestroyRenderer(renderer);
    }
error_create_surface:
    if (opts.draw) {
        SDL_DestroyWindow(window);
    }
error_create_window:
    SDL_Quit();
error_sdl_init:
    exit(status);
}
