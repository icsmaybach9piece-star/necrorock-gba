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

    /*
        0 = completely solid
        1 = one-way platform

        One-way platforms:
        - can be jumped through from below
        - can be landed on from above
        - do not block horizontal movement
    */
    int one_way;
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

/*
    Checks walls and fully-solid geometry.

    One-way platforms are deliberately NOT included here.
*/
int world_collides(
    int x,
    int y,
    int width,
    int height
);

/*
    Checks vertical movement.

    Returns:
        -1 if there is no collision
        otherwise the Y coordinate of the surface
        Iggy should stand on.

    One-way platforms only collide when Iggy is falling
    through their top surface.
*/
int world_vertical_collision(
    int x,
    int current_y,
    int next_y,
    int width,
    int height,
    int velocity_y
);

#endif
