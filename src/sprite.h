#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

#define SPRITE_TRANSPARENT 0
#define SPRITE_SKIN        1
#define SPRITE_HAIR        2
#define SPRITE_JACKET      3
#define SPRITE_RED         4
#define SPRITE_BOOT        5
#define SPRITE_METAL       6
#define SPRITE_EYE         7
#define SPRITE_WHITE       8

void sprite_draw(
    const uint8_t *sprite,
    int width,
    int height,
    int x,
    int y,
    int scale
);

#endif
