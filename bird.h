#ifndef BIRD_H
#define BIRD_H

#define WORLD_W 300.0f
#define WORLD_H 400.0f
#define BIRD_X 60.0f

#define GRAVITY 1.0f
#define FLAP_VY -10.0f
#define MAX_VY 15.0f

#define PIPE_SPEED 4.0f
#define PIPE_WIDTH 40.0f
#define PIPE_GAP 100.0f

typedef struct {
    float bird_y;
    float bird_vy;
    float pipe_x;
    float pipe_gap_y;
    int score;
    int game_over;
} BirdGame;

void bird_reset(BirdGame *g);

float bird_step(BirdGame *g, int action);

void bird_get_state(const BirdGame *g, float *dx, float *dy, float *vy);

void bird_render(const BirdGame *g);

#endif
