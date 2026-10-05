#include <stdint.h>

#include "../include/gba.h"
#include "sprite.h"

static uint16_t palette_color(char c)
{
    switch (c)
    {
        /* Bare skin / torso */
        case 'S':
            return RGB15(24, 14, 10);

        /* Blonde hair */
        case 'Y':
            return RGB15(31, 24, 8);

        /* Dark pants */
        case 'J':
            return RGB15(3, 3, 4);

        /* Red punk accent */
        case 'R':
            return RGB15(28, 4, 5);

        /* Boots */
        case 'B':
            return RGB15(1, 1, 2);

        /* Highlight */
        case 'W':
            return RGB15(31, 31, 28);

        default:
            return RGB15(0, 0, 0);
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

            /*
             * Periods are transparent.
             */
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
