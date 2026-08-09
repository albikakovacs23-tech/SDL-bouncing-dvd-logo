#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int main() {
    time_t t;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(   //this is gonna creat a window
        "DVD",
        800, 600,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Surface *surface = SDL_LoadBMP("dvd-logo.bmp");

    if (surface == NULL) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
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

    bool running = true;
    SDL_Event event;

    float x = 300;
    float y = 200;

    float dx = 0.1;
    float dy = 0.1; 

    srand(time(&t));        //this is the random color picker
    int colors[][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 0},
        {255, 255, 255}
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);
    int random_colors = rand() % num_colors;
    int number;

    while (running) {      //this is the main func

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            
        }

        x += dx;
        y += dy;

        if ( x <= 0 || x + 250 >= 800 ) {
            dx = -dx;
            
        int color = rand() % num_colors;

        SDL_SetTextureColorMod(
            texture,
            colors[color][0],
            colors[color][1],
            colors[color][2]
        );
    }
        

        if ( y <= 0 || y + 117 >= 600 ) {
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

        SDL_FRect dst = { x, y, 250, 117 };

        SDL_RenderTexture(renderer, texture, NULL, &dst);

        SDL_RenderPresent(renderer);
        }
SDL_DestroyTexture(texture);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);

SDL_Quit();
return 0;
}