#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#define WORLD_WIDTH  960
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
