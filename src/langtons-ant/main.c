#include <ncurses.h>
#include <unistd.h> // For usleep function

#define GRID_SIZE 40
#define DELAY 50000 // Delay in microseconds

typedef enum { WHITE, BLACK } Color;
typedef enum { UP, RIGHT, DOWN, LEFT } Direction;

typedef struct {
    int x;
    int y;
    Direction direction;
} Ant;

void turn_right(Ant *ant) {
    ant->direction = (ant->direction + 1) % 4;
}

void turn_left(Ant *ant) {
    ant->direction = (ant->direction + 3) % 4; // Adding 3 is equivalent to subtracting 1 mod 4
}

void move_forward(Ant *ant) {
    switch (ant->direction) {
        case UP:
            ant->y--;
            break;
        case RIGHT:
            ant->x++;
            break;
        case DOWN:
            ant->y++;
            break;
        case LEFT:
            ant->x--;
            break;
    }
}

void draw_grid(Color grid[GRID_SIZE][GRID_SIZE], Ant *ant) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (x == ant->x && y == ant->y) {
                mvaddch(y, x, 'A');
            } else {
                mvaddch(y, x, grid[y][x] == WHITE ? '.' : '#');
            }
        }
    }
}

int main() {
    Color grid[GRID_SIZE][GRID_SIZE] = { WHITE };
    Ant ant = { GRID_SIZE / 2, GRID_SIZE / 2, UP };

    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);

    while (1) {
        clear();
        if (grid[ant.y][ant.x] == WHITE) {
            turn_right(&ant);
            grid[ant.y][ant.x] = BLACK;
        } else {
            turn_left(&ant);
            grid[ant.y][ant.x] = WHITE;
        }
        move_forward(&ant);

        // Ensure the ant stays within the bounds of the grid
        if (ant.x < 0) ant.x = GRID_SIZE - 1;
        if (ant.x >= GRID_SIZE) ant.x = 0;
        if (ant.y < 0) ant.y = GRID_SIZE - 1;
        if (ant.y >= GRID_SIZE) ant.y = 0;

        draw_grid(grid, &ant);
        refresh();
        usleep(DELAY);
    }

    endwin();
    return 0;
}

