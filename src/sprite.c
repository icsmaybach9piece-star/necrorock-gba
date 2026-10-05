#include <stdint.h>

#include "../include/gba.h"
#include "sprite.h"

static uint16_t palette_color(char c)
{
    switch (c)
    {
        /* Skin */
        case 'S':
            return RGB15(25, 15, 11);

        /* Skin shadow */
        case 's':
            return RGB15(18, 9, 7);

        /* Blonde hair */
        case 'Y':
            return RGB15(31, 25, 9);

        /* Hair shadow */
        case 'y':
            return RGB15(22, 16, 5);

        /* Pants */
        case 'J':
            return RGB15(3, 3, 4);

        /* Pants highlight */
        case 'j':
            return RGB15(7, 7, 8);

        /* Boots */
        case 'B':
            return RGB15(1, 1, 2);

        /* Red punk accent */
        case 'R':
            return RGB15(27, 4, 5);

        /* Bright highlight */
        case 'W':
            return RGB15(31, 30, 25);

        default:
            return 0;
    }
}

void sprite_draw(
    const char *sprite,
    int width,
    int height,
    int x,
    int y,
    int scale
)
{
    if (sprite == 0)
        return;

    if (scale < 1)
        scale = 1;

    for (int py = 0; py < height; ++py)
    {
        for (int px = 0; px < width; ++px)
        {
            char pixel = sprite[py * width + px];

            if (pixel == '.')
                continue;

            uint16_t color = palette_color(pixel);

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
