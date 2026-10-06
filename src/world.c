#include <stdint.h>

#include "../include/gba.h"

#include "world.h"

/*
    ==========================================================
    NECROROCK WORLD 01
    ==========================================================

    Current prototype world:

        0 - 384      NECRO-CHAPEL
        384 - 768    BONE TUNNELS
        768 - 1152   CATACOMBS
        1152 - 1536  ORGAN WORKS
        1536 - 1920  FLESH PIT

    Exploration systems:

        - persistent secret
        - breakable barrier
        - hidden passage
        - shortcut state
*/

static GameState *game_state;


/*
    ==========================================================
    MAIN GEOMETRY
    ==========================================================
*/

static const Solid solids[] =
{
    /*
        ======================================================
        MAIN FLOOR
        ======================================================
    */

    {    0, 136, 1920, 24 },


    /*
        ======================================================
        NECRO-CHAPEL
        ======================================================
    */

    {   45, 108, 100, 10 },
    {  210,  92, 100, 10 },

    /*
        Secret-room climbing route.

        The lower platform allows Iggy to reach
        the upper platform with the current jump.
    */

    {  300,  98,  55, 10 },
    {  300,  65,  55, 10 },


    /*
        ======================================================
        BONE TUNNELS
        ======================================================
    */

    {  410, 105, 120, 10 },
    {  575,  82, 110, 10 },
    {  690, 110,  50, 10 },

    {  470,  55,  55, 10 },


    /*
        ======================================================
        CATACOMBS
        ======================================================
    */

    {  785, 105, 100, 10 },
    {  925,  78,  80, 10 },
    { 1045,  52,  65, 10 },

    {  830,  48,  45, 10 },


    /*
        ======================================================
        ORGAN WORKS
        ======================================================
    */

    { 1175, 110, 100, 10 },
    { 1310,  88, 110, 10 },
    { 1450, 112,  65, 10 },

    { 1220,  55,  50, 10 },
    { 1380,  48,  60, 10 },


    /*
        ======================================================
        FLESH PIT
        ======================================================
    */

    { 1550, 105, 100, 10 },
    { 1690,  78, 100, 10 },
    { 1810, 100,  75, 10 },

    { 1610,  50,  60, 10 },
    { 1760,  42,  55, 10 }
};

#define SOLID_COUNT \
    (sizeof(solids) / sizeof(solids[0]))


/*
    ==========================================================
    SPECIAL EXPLORATION OBJECTS
    ==========================================================
*/


/*
    First secret.

    Located above the Necro-Chapel platform.
*/

#define SECRET_HP_X        326
#define SECRET_HP_Y         40
#define SECRET_HP_WIDTH      8
#define SECRET_HP_HEIGHT     8


/*
    First breakable barrier.

    It blocks a small passage between the
    Necro-Chapel and Bone Tunnels.
*/

#define BARRIER_X           360
#define BARRIER_Y           104
#define BARRIER_WIDTH        12
#define BARRIER_HEIGHT       32


/*
    ==========================================================
    COLLISION HELPERS
    ==========================================================
*/

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


/*
    ==========================================================
    WORLD INITIALIZATION
    ==========================================================
*/

void world_init(GameState *state)
{
    game_state = state;
}


/*
    ==========================================================
    WORLD UPDATE
    ==========================================================
*/

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
)
{
    if (game_state == 0)
        return;


    /*
        ======================================================
        BREAKABLE BARRIER
        ======================================================
    */

    if (!game_state->barrier_01_destroyed &&
        player_attacking)
    {
        if (overlap(
                attack_x,
                attack_width,
                BARRIER_X,
                BARRIER_WIDTH) &&
            overlap(
                attack_y,
                attack_height,
                BARRIER_Y,
                BARRIER_HEIGHT))
        {
            game_state->barrier_01_destroyed = 1;
            game_state->shortcut_01_open = 1;
        }
    }


    /*
        ======================================================
        SECRET HP UPGRADE
        ======================================================
    */

    if (!game_state->secret_hp_01)
    {
        if (overlap(
                player_x,
                player_width,
                SECRET_HP_X,
                SECRET_HP_WIDTH) &&
            overlap(
                player_y,
                player_height,
                SECRET_HP_Y,
                SECRET_HP_HEIGHT))
        {
            game_state->secret_hp_01 = 1;
        }
    }
}


/*
    ==========================================================
    COLLISION
    ==========================================================
*/

int world_collides(
    int x,
    int y,
    int width,
    int height
)
{
    /*
        Normal world geometry.
    */

    for (unsigned int i = 0;
         i < SOLID_COUNT;
         ++i)
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


    /*
        Breakable barrier.

        It disappears permanently after destruction.
    */

    if (game_state != 0 &&
        !game_state->barrier_01_destroyed)
    {
        if (overlap(
                x,
                width,
                BARRIER_X,
                BARRIER_WIDTH) &&
            overlap(
                y,
                height,
                BARRIER_Y,
                BARRIER_HEIGHT))
        {
            return 1;
        }
    }

    return 0;
}


/*
    ==========================================================
    REGION BACKGROUNDS
    ==========================================================
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

    for (int x = start;
         x < end;
         x += 48)
    {
        if (x < -20 || x >= 240)
            continue;

        int height =
            65 + ((x + region_x) % 35);

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

        if (type == 0 ||
            type == 2 ||
            type == 4)
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
        ======================================================
        NECRO-CHAPEL
        ======================================================
    */

    if (type == 0)
    {
        for (int x = start + 18;
             x < end;
             x += 96)
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


    /*
        ======================================================
        BONE TUNNELS
        ======================================================
    */

    else if (type == 1)
    {
        for (int x = start;
             x < end;
             x += 64)
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


    /*
        ======================================================
        CATACOMBS
        ======================================================
    */

    else if (type == 2)
    {
        for (int x = start + 24;
             x < end;
             x += 72)
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


    /*
        ======================================================
        ORGAN WORKS
        ======================================================
    */

    else if (type == 3)
    {
        for (int x = start + 20;
             x < end;
             x += 80)
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


    /*
        ======================================================
        FLESH PIT
        ======================================================
    */

    else
    {
        for (int x = start;
             x < end;
             x += 56)
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


/*
    ==========================================================
    WORLD DRAW
    ==========================================================
*/

void world_draw(int camera_x)
{
    gba_clear(
        RGB15(1, 1, 2)
    );


    /*
        ======================================================
        FIVE REGIONS
        ======================================================
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
        ======================================================
        PLATFORMS AND FLOOR
        ======================================================
    */

    for (unsigned int i = 0;
         i < SOLID_COUNT;
         ++i)
    {
        const Solid *s = &solids[i];

        int sx =
            s->x - camera_x;

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
        ======================================================
        BREAKABLE BARRIER
        ======================================================
    */

    if (game_state != 0 &&
        !game_state->barrier_01_destroyed)
    {
        int bx =
            BARRIER_X - camera_x;

        if (bx + BARRIER_WIDTH >= 0 &&
            bx < 240)
        {
            gba_rect(
                bx,
                BARRIER_Y,
                BARRIER_WIDTH,
                BARRIER_HEIGHT,
                RGB15(10, 8, 8)
            );


            /*
                Organic red cracks.
            */

            gba_rect(
                bx + 3,
                BARRIER_Y + 5,
                2,
                22,
                RGB15(18, 3, 4)
            );

            gba_rect(
                bx + 7,
                BARRIER_Y + 14,
                2,
                14,
                RGB15(20, 3, 4)
            );
        }
    }


    /*
        ======================================================
        SECRET HP UPGRADE
        ======================================================
    */

    if (game_state != 0 &&
        !game_state->secret_hp_01)
    {
        int sx =
            SECRET_HP_X - camera_x;

        if (sx + SECRET_HP_WIDTH >= 0 &&
            sx < 240)
        {
            /*
                Small red/white core.
            */

            gba_rect(
                sx + 2,
                SECRET_HP_Y + 2,
                4,
                4,
                RGB15(25, 4, 5)
            );

            gba_rect(
                sx,
                SECRET_HP_Y + 3,
                8,
                2,
                RGB15(15, 12, 12)
            );
        }
    }


    /*
        ======================================================
        FLOOR MARKINGS
        ======================================================
    */

    for (int x = -camera_x;
         x < WORLD_WIDTH;
         x += 24)
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
