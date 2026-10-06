#include <stdint.h>

#include "../include/gba.h"
#include "world.h"

/*
    NECROROCK WORLD 01

    Temporary geometry.

    The important thing here is the layout:
    multiple elevations, gaps, routes and
    landmarks that will later become real
    biomechanical environments.
*/

static const Solid solids[] =
{
    /*
        ==================================================
        MAIN FLOOR
        ==================================================
    */

    {    0, 136, 1920, 24 },


    /*
        ==================================================
        NECRO-CHAPEL
        0 - 384
        ==================================================
    */

    {   45, 108, 100, 10 },
    {  210,  92, 100, 10 },
    {  300,  65,  55, 10 },


    /*
        ==================================================
        BONE TUNNELS
        384 - 768
        ==================================================
    */

    {  410, 105, 120, 10 },
    {  575,  82, 110, 10 },
    {  690, 110,  50, 10 },

    {  470,  55,  55, 10 },


    /*
        ==================================================
        CATACOMBS
        768 - 1152
        ==================================================
    */

    {  785, 105, 100, 10 },
    {  925,  78,  80, 10 },
    { 1045,  52,  65, 10 },

    {  830,  48,  45, 10 },


    /*
        ==================================================
        ORGAN WORKS
        1152 - 1536
        ==================================================
    */

    { 1175, 110, 100, 10 },
    { 1310,  88, 110, 10 },
    { 1450, 112, 65, 10 },

    { 1220,  55,  50, 10 },
    { 1380,  48, 60, 10 },


    /*
        ==================================================
        FLESH PIT
        1536 - 1920
        ==================================================
    */

    { 1550, 105, 100, 10 },
    { 1690,  78, 100, 10 },
    { 1810, 100, 75, 10 },

    { 1610,  50, 60, 10 },
    { 1760,  42, 55, 10 }
};

#define SOLID_COUNT \
    (sizeof(solids) / sizeof(solids[0]))

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

        if (overlap(
                x,
                width,
                s->x,
                s->width) &&
            overlap(
                y,
                height,
                s->y,
                s->height))
        {
            return 1;
        }
    }

    return 0;
}

/*
    Draw temporary world geometry.

    Each region gets a slightly different
    architectural pattern so we can visually
    tell where we are even before the final
    art pass.
*/

static void draw_region_background(
    int camera_x,
    int region_x,
    int region_width,
    int type
)
{
    int start = region_x - camera_x;
    int end = start + region_width;

    if (end < 0 || start >= 240)
        return;

    /*
        Region base.
    */

    gba_rect(
        start,
        0,
        region_width,
        136,
        RGB15(1, 1, 2)
    );

    /*
        Vertical biomechanical structures.
    */

    for (int x = start; x < end; x += 48)
    {
        if (x < -20 || x >= 240)
            continue;

        int height = 65 + ((x + region_x) % 35);

        gba_rect(
            x,
            136 - height,
            14,
            height,
            RGB15(4, 5, 6)
        );

        /*
            Organic red vein.
        */

        if (type == 0 || type == 2 || type == 4)
        {
            gba_rect(
                x + 5,
                136 - height + 8,
                2,
                height - 12,
                RGB15(10, 2, 3)
            );
        }
    }

    /*
        Region-specific details.
    */

    if (type == 0)
    {
        /* Chapel pillars */
        for (int x = start + 18; x < end; x += 96)
        {
            if (x >= 0 && x < 240)
            {
                gba_rect(
                    x,
                    40,
                    8,
                    96,
                    RGB15(7, 7, 8)
                );
            }
        }
    }
    else if (type == 1)
    {
        /* Bone-like horizontal structures */
        for (int x = start; x < end; x += 64)
        {
            if (x >= -20 && x < 240)
            {
                gba_rect(
                    x,
                    45,
                    38,
                    4,
                    RGB15(9, 9, 9)
                );

                gba_rect(
                    x + 8,
                    49,
                    4,
                    18,
                    RGB15(7, 7, 7)
                );
            }
        }
    }
    else if (type == 2)
    {
        /* Catacomb arches */
        for (int x = start + 24; x < end; x += 72)
        {
            if (x >= -20 && x < 240)
            {
                gba_rect(
                    x,
                    35,
                    5,
                    75,
                    RGB15(8, 8, 8)
                );

                gba_rect(
                    x + 31,
                    35,
                    5,
                    75,
                    RGB15(8, 8, 8)
                );

                gba_rect(
                    x,
                    35,
                    36,
                    5,
                    RGB15(8, 8, 8)
                );
            }
        }
    }
    else if (type == 3)
    {
        /* Machinery */
        for (int x = start + 20; x < end; x += 80)
        {
            if (x >= -30 && x < 240)
            {
                gba_rect(
                    x,
                    65,
                    30,
                    30,
                    RGB15(5, 6, 7)
                );

                gba_rect(
                    x + 6,
                    71,
                    18,
                    18,
                    RGB15(9, 3, 4)
                );
            }
        }
    }
    else
    {
        /* Flesh-pit ribs */
        for (int x = start; x < end; x += 56)
        {
            if (x >= -20 && x < 240)
            {
                gba_rect(
                    x,
                    20,
                    5,
                    116,
                    RGB15(9, 5, 5)
                );

                gba_rect(
                    x + 18,
                    40,
                    3,
                    96,
                    RGB15(12, 4, 4)
                );
            }
        }
    }
}

void world_draw(int camera_x)
{
    gba_clear(RGB15(1, 1, 2));

    /*
        Five regions.
    */

    draw_region_background(
        camera_x,
        0,
        384,
        0
    );

    draw_region_background(
        camera_x,
        384,
        384,
        1
    );

    draw_region_background(
        camera_x,
        768,
        384,
        2
    );

    draw_region_background(
        camera_x,
        1152,
        384,
        3
    );

    draw_region_background(
        camera_x,
        1536,
        384,
        4
    );

    /*
        Platforms and floor.
    */

    for (unsigned int i = 0; i < SOLID_COUNT; ++i)
    {
        const Solid *s = &solids[i];

        int sx = s->x - camera_x;

        if (sx + s->width < 0 ||
            sx >= 240)
        {
            continue;
        }

        gba_rect(
            sx,
            s->y,
            s->width,
            s->height,
            RGB15(6, 7, 8)
        );

        /*
            Red organic edge.
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
        Floor markings.
    */

    for (int x = -camera_x; x < WORLD_WIDTH; x += 24)
    {
        if (x < -10 || x > 240)
            continue;

        gba_rect(
            x,
            132,
            3,
            4,
            RGB15(11, 12, 12)
        );
    }
}
