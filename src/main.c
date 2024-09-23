#include "SDL.h"
#include <stdio.h>

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 480

#define CELL_HEIGHT 10
#define CELL_WIDTH 10

void Initialization();
void Render();
void Cleanup();

SDL_Window *window = NULL;
SDL_Renderer *renderer;

int main(void) {
    Initialization();

    Render();

    Cleanup();
    return 0;
}

void Initialization() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "Conway's Game Of Life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_HEIGHT, WINDOW_WIDTH,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

int running = 1;

void Render() {
    SDL_Rect rect[2];
    rect[0].x = 100;
    rect[0].y = 50;
    rect[0].h = CELL_HEIGHT;
    rect[0].w = CELL_WIDTH;
    rect[1].x = 200;
    rect[1].y = 50;
    rect[1].h = CELL_HEIGHT;
    rect[1].w = CELL_WIDTH;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = 0;
                }
            }
        }
        // Draw background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw cells
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < sizeof(rect) / sizeof(SDL_Rect); i++) {
            SDL_RenderFillRect(renderer, &rect[i]);
        }

        // Render
        SDL_RenderPresent(renderer);
    }
}

void Cleanup() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
