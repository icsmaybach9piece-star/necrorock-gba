#include <stdint.h>
#include "../include/gba.h"
#include "world.h"

static const Solid solids[] =
{
    /* Main floor */
    { 0,   136, 960, 24 },

    /* Platforms */
    { 90,  108, 110, 10 },
    { 270, 92,  120, 10 },
    { 470, 112, 130, 10 },
    { 690, 82,  110, 10 },

    /* Raised structures */
    { 155, 74,  45,  10 },
    { 360, 58,  55,  10 },
    { 815, 105, 90,  10 }
};

#define SOLID_COUNT (sizeof(solids) / sizeof(solids[0]))

void world_init(void)
{
}

static int overlap(
    int a,
    int size_a,
    int b,
    int size_b
)
{
    return a < b + size_b &&
           a + size_a > b;
}

int world_collides(
    int x,
    int y,
    int width,
    int height
)
{
    for (unsigned int i = 0; i < SOLID_COUNT; ++i)
    {
        const Solid *s = &solids[i];

        if (overlap(x, width, s->x, s->width) &&
            overlap(y, height, s->y, s->height))
        {
            return 1;
        }
    }

    return 0;
}

void world_draw(int camera_x)
{
    gba_clear(RGB15(1, 1, 2));

    /*
       Background void
    */

    gba_rect(
        0,
        0,
        240,
        160,
        RGB15(1, 1, 2)
    );

    /*
       Biomechanical background columns
    */

    for (int x = -camera_x; x < WORLD_WIDTH; x += 120)
    {
        int screen_x = x;

        if (screen_x < -30 || screen_x > 240)
            continue;

        gba_rect(
            screen_x,
            30,
            18,
            106,
            RGB15(5, 6, 7)
        );

        gba_rect(
            screen_x + 6,
            38,
            3,
            90,
            RGB15(12, 3, 4)
        );
    }

    /*
       Draw solid geometry
    */

    for (unsigned int i = 0; i < SOLID_COUNT; ++i)
    {
        const Solid *s = &solids[i];

        int sx = s->x - camera_x;

        if (sx + s->width < 0 || sx >= 240)
            continue;

        /*
           Dark organic structure
        */

        gba_rect(
            sx,
            s->y,
            s->width,
            s->height,
            RGB15(6, 7, 8)
        );

        /*
           Red biomechanical veins
        */

        if (s->height <= 12)
        {
            gba_rect(
                sx + 4,
                s->y,
                s->width - 8,
                2,
                RGB15(14, 3, 4)
            );
        }
    }

    /*
       Floor teeth / alien growths
    */

    for (int x = -camera_x; x < WORLD_WIDTH; x += 24)
    {
        int sx = x;

        if (sx < -10 || sx > 240)
            continue;

        gba_rect(
            sx,
            132,
            3,
            4,
            RGB15(11, 12, 12)
        );
    }
}
