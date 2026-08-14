#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    const int window_width = 800;
    const int window_height = 600;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface *surface = SDL_LoadBMP("dvd-logo.bmp");

    if (surface == NULL) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const int logo_width = surface->w;
    const int logo_height = surface->h;

    SDL_Window *window = SDL_CreateWindow(
        "DVD",
        window_width,
        window_height,
        0
    );

    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (texture == NULL) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
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

    const int num_colors = sizeof(colors) / sizeof(colors[0]);

    srand((unsigned int)time(NULL));

    bool running = true;
    SDL_Event event;

    float x = 300.0f;
    float y = 200.0f;

    float dx = 0.1f;
    float dy = 0.1f;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        x += dx;
        y += dy;

        if (x <= 0 || x + logo_width >= window_width) {
            dx = -dx;

            int color = rand() % num_colors;

            SDL_SetTextureColorMod(
                texture,
                colors[color][0],
                colors[color][1],
                colors[color][2]
            );
        }

        if (y <= 0 || y + logo_height >= window_height) {
            dy = -dy;

            int color = rand() % num_colors;

            SDL_SetTextureColorMod(
                texture,
                colors[color][0],
                colors[color][1],
                colors[color][2]
            );
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_FRect dst = {
            x,
            y,
            (float)logo_width,
            (float)logo_height
        };

        SDL_RenderTexture(renderer, texture, NULL, &dst);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
