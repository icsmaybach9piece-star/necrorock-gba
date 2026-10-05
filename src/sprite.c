#include <stdint.h>

#include "../include/gba.h"
#include "sprite.h"

static uint16_t palette_color(uint8_t index)
{
    switch (index)
    {
        case SPRITE_SKIN:
            return RGB15(22, 10, 8);

        case SPRITE_HAIR:
            return RGB15(2, 2, 2);

        case SPRITE_JACKET:
            return RGB15(6, 6, 7);

        case SPRITE_RED:
            return RGB15(22, 3, 4);

        case SPRITE_BOOT:
            return RGB15(1, 1, 1);

        case SPRITE_METAL:
            return RGB15(12, 13, 14);

        case SPRITE_EYE:
            return RGB15(31, 2, 3);

        case SPRITE_WHITE:
            return RGB15(28, 28, 26);

        default:
            return RGB15(0, 0, 0);
    }
}

void sprite_draw(
    const uint8_t *sprite,
    int width,
    int height,
    int x,
    int y,
    int scale
)
{
    for (int py = 0; py < height; ++py)
    {
        for (int px = 0; px < width; ++px)
        {
            uint8_t value = sprite[py * width + px];

            if (value == SPRITE_TRANSPARENT)
                continue;

            uint16_t color = palette_color(value);

            for (int sy = 0; sy < scale; ++sy)
            {
                for (int sx = 0; sx < scale; ++sx)
                {
                    gba_pixel(
                        x + px * scale + sx,
                        y + py * scale + sy,
                        color
                    );
                }
            }
        }
    }
}
