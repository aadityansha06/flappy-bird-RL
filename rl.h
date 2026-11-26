#ifndef RL_H
#define RL_H

#define DX_BINS 15
#define DY_BINS 20
#define VY_BINS 5
#define ACTIONS 2

#define ALPHA 0.1f
#define GAMMA 0.95f

typedef struct {
    int dx;
    int dy;
    int vy;
} AgentState;

extern float Qtable[DX_BINS][DY_BINS][VY_BINS][ACTIONS];

AgentState agent_livestate(float raw_dx, float raw_dy, float raw_vy);
int agent_choose_action(AgentState s, float epsilon);
void agent_update(AgentState s, int action, float reward, AgentState next_s, int game_over);

void rl_save(const char *filename);
int rl_load(const char *filename);

#endif
