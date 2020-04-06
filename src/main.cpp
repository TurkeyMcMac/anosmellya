#include "World.hpp"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace anosmellya;

static const unsigned WORLD_WIDTH = 300;
static const unsigned WORLD_HEIGHT = 150;

static bool is_quit_event(SDL_Event& event)
{
    return event.type == SDL_QUIT
        || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_q);
}

static void simulate(SDL_Renderer* renderer)
{
    SDL_Event event;
    SDL_Rect screen_dims;
    SDL_RenderGetViewport(renderer, &screen_dims);
    Random random((uint32_t)time(NULL));
    World world(WORLD_WIDTH, WORLD_HEIGHT, random, 0.1, 0.1);
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (is_quit_event(event)) {
                return;
            }
        }
        int mouse_x, mouse_y;
        if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)
            && mouse_x >= 0 && mouse_y >= 0) {
            world.set((unsigned)mouse_x / 4, (unsigned)mouse_y / 4, 100.);
        }
        world.draw(renderer);
        SDL_RenderPresent(renderer);
        world.simulate(random);
        Uint32 new_ticks = SDL_GetTicks();
        if (new_ticks - ticks < 60) {
            SDL_Delay(60 - (new_ticks - ticks));
        }
    }
}

int main(int argc, char* argv[])
{
    int status = EXIT_FAILURE;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Uint32 init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO;
    if (SDL_Init(init_flags)) {
        fprintf(stderr, "SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    {
        // Metal is amazingly slow for some reason; try using OpenGL instead:
        SDL_RendererInfo renderer_info;
        if (SDL_GetRenderDriverInfo(0, &renderer_info) == 0
            && !strcmp(renderer_info.name, "metal")) {
            SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        }
        window = SDL_CreateWindow("Intergrid", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, WORLD_WIDTH * 4, WORLD_HEIGHT * 4,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!window) {
            fprintf(stderr, "SDL window creation failed; %s\n", SDL_GetError());
            goto error_create_window;
        }
        /*
        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
        { fprintf(stderr, "SDL window fullscreenification failed; %s\n",
                SDL_GetError());
            // Keep executing.
        }
        */
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
    simulate(renderer);
    status = EXIT_SUCCESS;
    SDL_DestroyRenderer(renderer);
error_create_surface:
    SDL_DestroyWindow(window);
error_create_window:
    SDL_Quit();
error_sdl_init:
    exit(status);
}
