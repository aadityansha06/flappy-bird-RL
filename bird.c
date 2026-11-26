#include "bird.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
static float random_gap_y(void) {
    float min_y = PIPE_GAP * 0.75f;
    float max_y = WORLD_H - PIPE_GAP * 0.75f;
    float r = (float)rand() / (float)RAND_MAX;
    return min_y + r * (max_y - min_y);
}

void bird_reset(BirdGame *g) {
    g->bird_y = WORLD_H / 2.0f;
    g->bird_vy = 0.0f;
    g->pipe_x = WORLD_W;
    g->pipe_gap_y = random_gap_y();
    g->score = 0;
    g->game_over = 0;
}

float bird_step(BirdGame *g, int action) {
    if (g->game_over) {
        return 0.0f;
    }

    if (action == 1) {
        g->bird_vy = FLAP_VY;
    }

    g->bird_vy += GRAVITY;
    if (g->bird_vy > MAX_VY) {
        g->bird_vy = MAX_VY;
    }
    g->bird_y += g->bird_vy;
    g->pipe_x -= PIPE_SPEED;

    if (g->bird_y <= 0.0f || g->bird_y >= WORLD_H) {
        g->game_over = 1;
        return -100.0f;
    }

    float gap_top = g->pipe_gap_y - (PIPE_GAP / 2.0f);
    float gap_bottom = g->pipe_gap_y + (PIPE_GAP / 2.0f);

    if (BIRD_X >= g->pipe_x && BIRD_X <= g->pipe_x + PIPE_WIDTH) {
        if (g->bird_y < gap_top || g->bird_y > gap_bottom) {
            g->game_over = 1;
            return -100.0f;
        }
    }

    if (g->pipe_x + PIPE_WIDTH < BIRD_X) {
        g->score++;
        g->pipe_x = WORLD_W;
        g->pipe_gap_y = random_gap_y();
        return 10.0f;
    }

    return 1.0f;
}

void bird_get_state(const BirdGame *g, float *dx, float *dy, float *vy) {
    *dx = (g->pipe_x + PIPE_WIDTH) - BIRD_X;
    *dy = g->bird_y - g->pipe_gap_y;
    *vy = g->bird_vy;
}
static void print_spaces(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

void bird_render(const BirdGame *g) {
    const int cols = 44;
    const int rows = 20;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int pad_x = (w.ws_col > cols) ? (w.ws_col - cols) / 2 : 0;
    int pad_y = (w.ws_row > rows + 3) ? (w.ws_row - (rows + 3)) / 2 : 0;

    int bx = (int)((BIRD_X / WORLD_W) * cols);
    int by = (int)((g->bird_y / WORLD_H) * rows);
    int px_start = (int)((g->pipe_x / WORLD_W) * cols);
    int px_end = (int)(((g->pipe_x + PIPE_WIDTH) / WORLD_W) * cols);
    int gap_top = (int)(((g->pipe_gap_y - PIPE_GAP / 2.0f) / WORLD_H) * rows);
    int gap_bot = (int)(((g->pipe_gap_y + PIPE_GAP / 2.0f) / WORLD_H) * rows);

    printf("\033[H\033[J");

    for (int i = 0; i < pad_y; i++) putchar('\n');

    print_spaces(pad_x);
    printf("Score: %d  (SPACE = flap, Q = quit)\n", g->score);

    print_spaces(pad_x);
    for (int c = 0; c < cols; c++) putchar('-');
    putchar('\n');

    for (int r = 0; r < rows; r++) {
        print_spaces(pad_x);
        for (int c = 0; c < cols; c++) {
            if (r == by && c == bx) {
                putchar('o');
            } else if (r == by && c == bx + 1) {
                putchar('>');
            } else if (c >= px_start && c <= px_end && (r < gap_top || r > gap_bot)) {
                int is_cap = (r == gap_top - 1 || r == gap_bot + 1);
                if (is_cap) {
                    putchar((c == px_start || c == px_end) ? '+' : '=');
                } else {
                    putchar((c == px_start || c == px_end) ? '|' : ' ');
                }
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }

    print_spaces(pad_x);
    for (int c = 0; c < cols; c++) putchar('-');
    putchar('\n');
}
