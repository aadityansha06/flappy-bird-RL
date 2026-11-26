#include "user-play.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "bird.h"


void manual_play(void) {
    srand(time(NULL));

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    int old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    int again = 0;

    do {
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);

        BirdGame g;
        bird_reset(&g);

        while (g.game_over == 0) {
            int action = 0;
            char c;

            while (read(STDIN_FILENO, &c, 1) > 0) {
                if (c == ' ') action = 1;
                if (c == 'q') g.game_over = 1;
            }

            bird_step(&g, action);
            bird_render(&g);
            usleep(60000);
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, old_flags);

        printf("\nGame Over! Final Score: %d\n", g.score);
        printf("Press 0 to play again or 1 to exit: ");
        if (scanf("%d", &again) != 1) {
            again = 1;
        }
    } while (again == 0);

    printf("\nBye bye\n");
}
