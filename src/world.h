#ifndef WORLD_H
#define WORLD_H

#include "game_state.h"

#define WORLD_WIDTH 1920

void world_init(GameState *state);

void world_update_platforms(void);

void world_update(
    int player_x,
    int player_y,
    int player_width,
    int player_height,
    int player_attacking,
    int attack_x,
    int attack_y,
    int attack_width,
    int attack_height
);

int world_collides(
    int x,
    int y,
    int width,
    int height
);

int world_vertical_collision(
    int x,
    int current_y,
    int next_y,
    int width,
    int height,
    int velocity_y
);

int world_platform_carry_x(
    int x,
    int y,
    int width,
    int height
);

int world_platform_carry_y(
    int x,
    int y,
    int width,
    int height
);

void world_draw(int camera_x);

#endif
