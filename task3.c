#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000 // Grid size N x N[cite: 1]
#define STEPS 5000 // Number of generations[cite: 1]

int grid[N][N];
int newgrid[N][N];

void initialize_grid() {
    srand(1234); // fixed seed for reproducibility[cite: 1]
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = rand() % 2;
        }
    }
}

int count_neighbors(int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}

void update() {
    // 1. Calculate the next generation
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int neighbors = count_neighbors(i, j);
            if (grid[i][j] == 1) {
                if (neighbors < 3) newgrid[i][j] = 0; // Rule 1[cite: 1]
                else if (neighbors == 3 || neighbors == 4) newgrid[i][j] = 1; // Rule 2[cite: 1]
                else newgrid[i][j] = 0; // Rule 3[cite: 1]
            } else {
                if (neighbors == 2) newgrid[i][j] = 1; // Rule 4[cite: 1]
                else newgrid[i][j] = 0;
            }
        }
    }

    // 2. Overwrite the old grid with the new states
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = newgrid[i][j];
        }
    }
}

int main() {
    initialize_grid();
    
    double start = omp_get_wtime();
    for (int step = 0; step < STEPS; step++) {
        update();
    }
    double end = omp_get_wtime();
    
    printf("Simulation finished in %f seconds\n", end - start);
    return 0;
}
