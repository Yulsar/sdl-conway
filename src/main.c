#include "SDL.h"
#include <stdio.h>
#include <malloc.h>

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 480

#define GRID_HEIGHT 640
#define GRID_WIDTH 480

#define CELL_HEIGHT 10
#define CELL_WIDTH 10

#define LOOP_DELAY 500

void Initialization();
void MainLoop();
void SeedGrid();
void CalculateGrid();
int GetXDir(int i);
int GetYDir(int i);
void GetDirs(int i, int *x, int *y);
void Cleanup();

SDL_Window *window = NULL;
SDL_Renderer *renderer;

int main(void) {
    Initialization();

    MainLoop();

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

int grid[GRID_WIDTH][GRID_HEIGHT];

void MainLoop() {
    SeedGrid();
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
        CalculateGrid();
        // Draw background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw cells
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                if (grid[x][y]) {
                    rect.x = x * CELL_WIDTH;
                    rect.y = y * CELL_HEIGHT;
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        // Render
        SDL_RenderPresent(renderer);
        SDL_Delay(LOOP_DELAY);
    }
}

void SeedGrid() {
    grid[4][6] = 1;
    grid[5][6] = 1;
    grid[6][6] = 1;
}


void CalculateGrid() {
    int grid_copy[GRID_WIDTH][GRID_HEIGHT];
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int neighbors = 0;
            for (int i = 0; i < 8; i++) {

                int x_dir = 0;
                int y_dir = 0;
                GetDirs(i, &x_dir, &y_dir);
                if (x + x_dir < 0 || x + x_dir >= GRID_WIDTH || y + y_dir < 0 || y + y_dir >= GRID_HEIGHT)
                continue;

                neighbors += grid[x + x_dir][y + y_dir];
            }
            if (grid[x][y] == 1) {
                if (neighbors < 2) {
                    grid_copy[x][y] = 0;
                }
                else if (neighbors == 2 || neighbors == 3) {
                    grid_copy[x][y] = 1;
                }
                else if (neighbors > 3) {
                    grid_copy[x][y] = 0;
                }
            }
            else {
                if (neighbors == 3) {
                    grid_copy[x][y] = 1;
                }
            }
        }
    }
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y] = grid_copy[x][y];
        }
    }
}

// 6 7 0
// 5   1
// 4 3 2

void GetDirs(int i, int *x, int *y) {
    if (i < 3) {
        *x = 1;
    }
    else if (i == 3 || i == 7) {
        *x = 0;
    }
    else {
        *x = -1;
    }

    if (i == 0 || i > 5) {
        *y = 1;
    }
    else if (i == 1 || i == 5) {
        *y = 0;
    }
    else {
        *y = -1;
    }
}

void Cleanup() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
