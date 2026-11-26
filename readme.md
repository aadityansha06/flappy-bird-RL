# Flappy Bird RL in Pure C

A terminal-based Flappy Bird game with an autonomous Tabular Q-Learning agent written from scratch in C—no Python, no neural networks, and no external ML libraries.

## How It Works
- **State Space:** Discretizes horizontal distance to pipe (`dx`), vertical distance to gap (`dy`), and vertical velocity (`vy`) into a `15 × 20 × 5 × 2` float Q-table (3,000 values / ~12 KB).
- **Learning:** Uses epsilon-greedy exploration and Temporal Difference (TD) Bellman updates (`alpha = 0.1`, `gamma = 0.95`).
- **Training:** Runs 20,000 headless episodes with optional milestone rendering, then exports the trained Q-table to `qtable.bin`.

## Project Structure
- `bird.c / bird.h` — Game physics, state extraction, and terminal rendering.
- `rl.c / rl.h` — Q-table, discretization, action selection, Bellman updates, and binary save/load.
- `user-play.c` — Manual keyboard gameplay.
- `main.c` — Menu, training loop, and AI playback.

## Build & Run
```bash
gcc -O3 main.c bird.c user-play.c rl.c -o flappy
./flappy
