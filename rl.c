#include <stdlib.h>
#include "rl.h"
#include <stdio.h>
float Qtable[DX_BINS][DY_BINS][VY_BINS][ACTIONS] = {0};

static int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

AgentState agent_livestate(float raw_dx, float raw_dy, float raw_vy) {
    AgentState s;
    s.dx = clamp((int)(raw_dx / 20.0f), 0, DX_BINS - 1);
    s.dy = clamp((int)((raw_dy + 200.0f) / 20.0f), 0, DY_BINS - 1);
    s.vy = clamp((int)((raw_vy + 10.0f) / 5.0f), 0, VY_BINS - 1);
    return s;
}

int agent_choose_action(AgentState s, float epsilon) {
    float r = (float)rand() / (float)RAND_MAX;
    if (r < epsilon) {
        return rand() % ACTIONS;
    }

    float q_nothing = Qtable[s.dx][s.dy][s.vy][0];
    float q_flap    = Qtable[s.dx][s.dy][s.vy][1];

    return (q_flap > q_nothing) ? 1 : 0;
}

void agent_update(AgentState s, int action, float reward, AgentState next_s, int game_over) {
    float max_next_q = 0.0f;

    if (!game_over) {
        float q0 = Qtable[next_s.dx][next_s.dy][next_s.vy][0];
        float q1 = Qtable[next_s.dx][next_s.dy][next_s.vy][1];
        max_next_q = (q1 > q0) ? q1 : q0;
    }

    float old_q = Qtable[s.dx][s.dy][s.vy][action];
    float target = reward + GAMMA * max_next_q;

    Qtable[s.dx][s.dy][s.vy][action] = old_q + ALPHA * (target - old_q);
}


void rl_save(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(Qtable, sizeof(Qtable), 1, f);
    fclose(f);
}

int rl_load(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    fread(Qtable, sizeof(Qtable), 1, f);
    fclose(f);
    return 1;
}
