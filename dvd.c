#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    const int window_width = 800;
    const int window_height = 600;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("Agopusb.ttf", 24);

    if (!font) {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *logo_surface = SDL_LoadBMP("dvd-logo.bmp");

    if (!logo_surface) {
        SDL_Log("SDL_LoadBMP failed: %s", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    const int logo_width = logo_surface->w;
    const int logo_height = logo_surface->h;

    SDL_Window *window = SDL_CreateWindow(
        "DVD",
        window_width,
        window_height,
        0
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_DestroySurface(logo_surface);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroySurface(logo_surface);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *logo_texture =
        SDL_CreateTextureFromSurface(renderer, logo_surface);

    SDL_DestroySurface(logo_surface);

    if (!logo_texture) {
        SDL_Log(
            "SDL_CreateTextureFromSurface failed: %s",
            SDL_GetError()
        );

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    const int colors[][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 0},
        {255, 255, 255}
    };

    const int color_count =
        sizeof(colors) / sizeof(colors[0]);

    srand((unsigned int)time(NULL));

    /*
     * Create the first FPS texture before entering the loop.
     * This makes "FPS: 0" visible immediately.
     */
    SDL_Texture *fps_texture = NULL;
    float fps_width = 0.0f;
    float fps_height = 0.0f;

    SDL_Surface *fps_surface = TTF_RenderText_Blended(
        font,
        "FPS: 0",
        0,
        (SDL_Color){255, 255, 255, 255}
    );

    if (!fps_surface) {
        SDL_Log(
            "TTF_RenderText_Blended failed: %s",
            SDL_GetError()
        );
    } else {
        fps_texture = SDL_CreateTextureFromSurface(
            renderer,
            fps_surface
        );

        if (!fps_texture) {
            SDL_Log(
                "Creating FPS texture failed: %s",
                SDL_GetError()
            );
        } else {
            fps_width = (float)fps_surface->w;
            fps_height = (float)fps_surface->h;
        }

        SDL_DestroySurface(fps_surface);
    }

    bool running = true;
    SDL_Event event;

    float x = 300.0f;
    float y = 200.0f;
    float dx = 100.0f;
    float dy = 100.0f;

    float fps_timer = 0.0f;
    int fps_count = 0;
    int fps = 0;

    Uint64 last_time = SDL_GetPerformanceCounter();

    while (running) {
        Uint64 current_time = SDL_GetPerformanceCounter();

        float delta_time =
            (float)(current_time - last_time) /
            (float)SDL_GetPerformanceFrequency();

        last_time = current_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        x += dx * delta_time;
        y += dy * delta_time;

        fps_timer += delta_time;
        fps_count++;

        /*
         * Update the FPS text once per second.
         */
        if (fps_timer >= 1.0f) {
            fps = fps_count;
            fps_count = 0;
            fps_timer -= 1.0f;

            char fps_text[32];

            SDL_snprintf(
                fps_text,
                sizeof(fps_text),
                "FPS: %d",
                fps
            );

            SDL_Surface *new_fps_surface =
                TTF_RenderText_Blended(
                    font,
                    fps_text,
                    0,
                    (SDL_Color){0, 0, 0, 255}
                );

            if (!new_fps_surface) {
                SDL_Log(
                    "Updating FPS text failed: %s",
                    SDL_GetError()
                );
            } else {
                SDL_Texture *new_fps_texture =
                    SDL_CreateTextureFromSurface(
                        renderer,
                        new_fps_surface
                    );

                if (!new_fps_texture) {
                    SDL_Log(
                        "Updating FPS texture failed: %s",
                        SDL_GetError()
                    );
                } else {
                    SDL_DestroyTexture(fps_texture);
                    fps_texture = new_fps_texture;

                    fps_width = (float)new_fps_surface->w;
                    fps_height = (float)new_fps_surface->h;
                }

                SDL_DestroySurface(new_fps_surface);
            }
        }

        if (x <= 0.0f ||
            x + logo_width >= window_width) {
            dx = -dx;

            int color = rand() % color_count;

            SDL_SetTextureColorMod(
                logo_texture,
                colors[color][0],
                colors[color][1],
                colors[color][2]
            );
        }

        if (y <= 0.0f ||
            y + logo_height >= window_height) {
            dy = -dy;

            int color = rand() % color_count;

            SDL_SetTextureColorMod(
                logo_texture,
                colors[color][0],
                colors[color][1],
                colors[color][2]
            );
        }

        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            255
        );

        SDL_RenderClear(renderer);

        if (fps_texture) {
            SDL_FRect fps_rect = {
                10.0f,
                10.0f,
                fps_width,
                fps_height
            };

            SDL_RenderTexture(
                renderer,
                fps_texture,
                NULL,
                &fps_rect
            );
        }

        SDL_FRect logo_rect = {
            x,
            y,
            (float)logo_width,
            (float)logo_height
        };

        SDL_RenderTexture(
            renderer,
            logo_texture,
            NULL,
            &logo_rect
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(fps_texture);
    SDL_DestroyTexture(logo_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
