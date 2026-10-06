#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

/*
    NECROROCK WORLD

    Large continuous prototype world.
    Later this will become a proper tile-based GBA world.

    8 screen widths = 1920 pixels.
*/

#define WORLD_WIDTH  1920
#define WORLD_HEIGHT 160

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Solid;

void world_init(void);

void world_draw(int camera_x);

int world_collides(
    int x,
    int y,
    int width,
    int height
);

#endif
