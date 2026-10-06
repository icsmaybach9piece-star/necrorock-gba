#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#include "game_state.h"

#define WORLD_WIDTH  1920
#define WORLD_HEIGHT 160

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Solid;

void world_init(GameState *state);

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

void world_draw(int camera_x);

int world_collides(
    int x,
    int y,
    int width,
    int height
);

#endif
