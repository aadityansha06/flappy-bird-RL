#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "bird.h"
#include "user-play.h"
#include "rl.h"

void RL(void);
void watch_ai(void);

int main(void) {
    int opt = -1;
    printf("\n1: Manual Play | 2: Train RL | 3: Watch Saved AI: ");
    scanf("%d", &opt);

    if (opt == 1) {
        manual_play();
    } else if (opt == 2) {
        RL();
    } else if (opt == 3) {
        if (rl_load("qtable.bin")) {
            watch_ai();
        } else {
            printf("No saved qtable.bin found. Train first (Option 2).\n");
        }
    } else {
        printf("exited..\n");
    }
    return 0;
}

void watch_ai(void) {
    BirdGame g;
    bird_reset(&g);

    while (g.game_over == 0) {
        float dx, dy, vy;
        bird_get_state(&g, &dx, &dy, &vy);
        AgentState state = agent_livestate(dx, dy, vy);

        int action = agent_choose_action(state, 0.0f);
        bird_step(&g, action);
        bird_render(&g);
        usleep(60000);
    }

    printf("\nAI Final Score: %d\n", g.score);
}

void RL(void) {
    srand(time(NULL));

    int epochs = 20000;
    float epsilon = 1.0f;
    float epsilon_min = 0.01f;
    float epsilon_decay = 0.9995f;
    int best_score = 0;

    for (int i = 0; i < epochs; i++) {
        BirdGame g;
        bird_reset(&g);

int watch = (i < 5) ||
            (i >= 500 && i < 503) ||
            (i >= 2000 && i < 2003) ||
            (i >= 10000 && i < 10002);

int steps = 0;
int max_steps = watch ? 300 : 5000;
while (g.game_over == 0 && steps < max_steps) {
    float dx, dy, vy;
    bird_get_state(&g, &dx, &dy, &vy);
    AgentState current_state = agent_livestate(dx, dy, vy);

    int action = agent_choose_action(current_state, epsilon);
    float reward = bird_step(&g, action);

    bird_get_state(&g, &dx, &dy, &vy);
    AgentState next_state = agent_livestate(dx, dy, vy);

    agent_update(current_state, action, reward, next_state, g.game_over);

    if (watch) {
        bird_render(&g);
        printf("Training Episode: %d | Epsilon: %.3f | Step: %d\n", i, epsilon, steps);
        usleep(55000);
    }
    steps++;
}

if (watch && g.game_over) {
    printf("CRASHED! (Score: %d)\n", g.score);
    usleep(500000); 
}

        if (g.score > best_score) {
            best_score = g.score;
        }

        if (epsilon > epsilon_min) {
            epsilon *= epsilon_decay;
        }

        if ((i + 1) % 1000 == 0) {
            printf("Episode %d | Best Score: %d | Epsilon: %.3f\n", i + 1, best_score, epsilon);
        }
    }

    rl_save("qtable.bin");
    printf("\nTraining done! Saved to qtable.bin. Watching trained AI...\n");
    sleep(2);

    watch_ai();
}
