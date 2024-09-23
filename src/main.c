#include "SDL.h"
#include <stdio.h>
#include <malloc.h>

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

typedef struct cell {
    int state;
    int x, y;
} cell;

void Render() {
    cell grid[2];
    grid[0].state = 1;
    grid[0].x = 100;
    grid[0].y = 0;
    grid[1].state = 1;
    grid[1].x = 200;
    grid[1].y = 0;
    
    SDL_Rect rect;
    rect.h = CELL_WIDTH;
    rect.w = CELL_HEIGHT;
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
        for (int i = 0; i < sizeof(grid) / sizeof(cell); i++) {
            if (grid[i].state) {
                rect.x = grid[i].x;
                rect.y = grid[i].y;
                grid[i].x += 1;

                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // Render
        SDL_RenderPresent(renderer);
    }
}

void Cleanup() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
