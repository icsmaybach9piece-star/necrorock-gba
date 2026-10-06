#include <stdint.h>

#include "../include/gba.h"

#include "world.h"


static GameState *game_state;


/*
    ============================================================
    BASIC SOLID GEOMETRY
    ============================================================

    one_way = 0
        Completely solid.

    one_way = 1
        Jump-through platform.
*/

static const Solid solids[] =
{
    /*
        Main floor.
    */
    {    0, 136, 1920, 24, 0 },


    /*
        ========================================================
        NECRO-CHAPEL
        ========================================================
    */

    {   45, 108, 100, 10, 1 },

    {  210, 112,  60, 10, 1 },

    {  300, 110,  55, 10, 1 },

    {  300,  82,  55, 10, 1 },


    /*
        ========================================================
        BONE TUNNELS
        ========================================================
    */

    {  410, 105, 120, 10, 1 },
    {  575,  82, 110, 10, 1 },
    {  690, 110,  50, 10, 1 },

    {  470,  55,  55, 10, 1 },


    /*
        ========================================================
        CATACOMBS
        ========================================================
    */

    {  785, 105, 100, 10, 1 },
    {  925,  78,  80, 10, 1 },
    { 1045,  52,  65, 10, 1 },

    {  830,  48,  45, 10, 1 },


    /*
        ========================================================
        ORGAN WORKS
        ========================================================
    */

    { 1175, 110, 100, 10, 1 },
    { 1310,  88, 110, 10, 1 },
    { 1450, 112,  65, 10, 1 },

    { 1220,  55,  50, 10, 1 },
    { 1380,  48,  60, 10, 1 },


    /*
        ========================================================
        FLESH PIT
        ========================================================
    */

    { 1550, 105, 100, 10, 1 },
    { 1690,  78, 100, 10, 1 },
    { 1810, 100,  75, 10, 1 },

    { 1610,  50,  60, 10, 1 },
    { 1760,  42,  55, 10, 1 }
};


#define SOLID_COUNT \
    (sizeof(solids) / sizeof(solids[0]))


/*
    ============================================================
    BREAKABLE OBJECT SYSTEM
    ============================================================

    Every breakable object has:

        x/y
        width/height
        hit points
        type

    The type is mostly for visual treatment.

        0 = barrier
        1 = wall
        2 = floor
        3 = ceiling
*/

typedef struct
{
    int x;
    int y;

    int width;
    int height;

    int hp;

    int type;

    int *destroyed;

} Breakable;


#define BREAKABLE_BARRIER  0
#define BREAKABLE_WALL     1
#define BREAKABLE_FLOOR    2
#define BREAKABLE_CEILING  3


/*
    Existing Chapel barrier.

    This remains exactly where the player already expects it.
*/
static Breakable breakables[] =
{
    {
        360,
        104,
        12,
        32,

        1,

        BREAKABLE_BARRIER,

        0
    }
};


#define BREAKABLE_COUNT \
    (sizeof(breakables) / sizeof(breakables[0]))


/*
    ============================================================
    SECRET UPGRADE
    ============================================================
*/

#define SECRET_HP_X        326
#define SECRET_HP_Y         48
#define SECRET_HP_WIDTH      8
#define SECRET_HP_HEIGHT     8


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
    Connect the breakable objects to persistent GameState.

    Keeping this in one place means adding a new breakable
    object later is straightforward.
*/
static void connect_breakables(void)
{
    if (game_state == 0)
        return;

    breakables[0].destroyed =
        &game_state->barrier_01_destroyed;
}


void world_init(GameState *state)
{
    game_state = state;

    connect_breakables();
}


/*
    ============================================================
    WORLD UPDATE
    ============================================================
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
        --------------------------------------------------------
        BREAKABLE OBJECTS
        --------------------------------------------------------

        Every breakable object receives the same attack logic.

        This is the part we can expand later without rewriting
        the player combat system.
    */

    if (player_attacking)
    {
        for (unsigned int i = 0;
             i < BREAKABLE_COUNT;
             ++i)
        {
            Breakable *object =
                &breakables[i];


            if (object->destroyed == 0)
                continue;


            if (*object->destroyed)
                continue;


            if (!overlap(
                    attack_x,
                    attack_width,
                    object->x,
                    object->width))
            {
                continue;
            }


            if (!overlap(
                    attack_y,
                    attack_height,
                    object->y,
                    object->height))
            {
                continue;
            }


            /*
                For now all prototype breakables require
                one successful attack.

                Later this becomes proper HP/damage.
            */
            object->hp--;


            if (object->hp <= 0)
            {
                *object->destroyed = 1;
            }
        }


        /*
            The first destroyed barrier opens the shortcut.
        */
        if (game_state->barrier_01_destroyed)
        {
            game_state->shortcut_01_open = 1;
        }
    }


    /*
        --------------------------------------------------------
        SECRET HP UPGRADE
        --------------------------------------------------------
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
    ============================================================
    NORMAL COLLISION
    ============================================================

    One-way platforms are ignored here.

    Breakables are checked separately because their collision
    depends on persistent destruction state.
*/

int world_collides(
    int x,
    int y,
    int width,
    int height
)
{
    /*
        Normal solid geometry.
    */

    for (unsigned int i = 0;
         i < SOLID_COUNT;
         ++i)
    {
        const Solid *s =
            &solids[i];


        if (s->one_way)
            continue;


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
        Breakable objects are completely solid while intact.
    */

    for (unsigned int i = 0;
         i < BREAKABLE_COUNT;
         ++i)
    {
        const Breakable *object =
            &breakables[i];


        if (object->destroyed == 0)
            continue;


        if (*object->destroyed)
            continue;


        if (overlap(
                x,
                width,
                object->x,
                object->width) &&
            overlap(
                y,
                height,
                object->y,
                object->height))
        {
            return 1;
        }
    }


    return 0;
}


/*
    ============================================================
    VERTICAL COLLISION
    ============================================================
*/

int world_vertical_collision(
    int x,
    int current_y,
    int next_y,
    int width,
    int height,
    int velocity_y
)
{
    int current_bottom =
        current_y + height;

    int next_bottom =
        next_y + height;


    /*
        --------------------------------------------------------
        FULLY SOLID GEOMETRY
        --------------------------------------------------------
    */

    for (unsigned int i = 0;
         i < SOLID_COUNT;
         ++i)
    {
        const Solid *s =
            &solids[i];


        if (s->one_way)
            continue;


        if (overlap(
                x,
                width,
                s->x,
                s->width) &&
            overlap(
                next_y,
                height,
                s->y,
                s->height))
        {
            if (velocity_y >= 0)
                return s->y;


            return s->y + s->height;
        }
    }


    /*
        --------------------------------------------------------
        BREAKABLE OBJECTS
        --------------------------------------------------------
    */

    for (unsigned int i = 0;
         i < BREAKABLE_COUNT;
         ++i)
    {
        const Breakable *object =
            &breakables[i];


        if (object->destroyed == 0)
            continue;


        if (*object->destroyed)
            continue;


        if (!overlap(
                x,
                width,
                object->x,
                object->width))
        {
            continue;
        }


        if (!overlap(
                next_y,
                height,
                object->y,
                object->height))
        {
            continue;
        }


        if (velocity_y >= 0)
            return object->y;


        return object->y + object->height;
    }


    /*
        --------------------------------------------------------
        ONE-WAY PLATFORMS
        --------------------------------------------------------
    */

    if (velocity_y > 0)
    {
        for (unsigned int i = 0;
             i < SOLID_COUNT;
             ++i)
        {
            const Solid *s =
                &solids[i];


            if (!s->one_way)
                continue;


            if (!overlap(
                    x,
                    width,
                    s->x,
                    s->width))
            {
                continue;
            }


            if (current_bottom <= s->y &&
                next_bottom >= s->y)
            {
                return s->y;
            }
        }
    }


    return -1;
}


/*
    ============================================================
    BACKGROUND
    ============================================================
*/

static void draw_region_background(
    int camera_x,
    int region_x,
    int region_width,
    int type
)
{
    int start =
        region_x - camera_x;

    int end =
        start + region_width;


    if (end < 0 || start >= 240)
        return;


    gba_rect(
        start,
        0,
        region_width,
        136,
        RGB15(1, 1, 2)
    );


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
    ============================================================
    WORLD DRAW
    ============================================================
*/

void world_draw(int camera_x)
{
    gba_clear(
        RGB15(1, 1, 2)
    );


    /*
        Region backgrounds.
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
        Normal geometry.
    */

    for (unsigned int i = 0;
         i < SOLID_COUNT;
         ++i)
    {
        const Solid *s =
            &solids[i];


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
        Breakable objects.
    */

    for (unsigned int i = 0;
         i < BREAKABLE_COUNT;
         ++i)
    {
        const Breakable *object =
            &breakables[i];


        if (object->destroyed == 0)
            continue;


        if (*object->destroyed)
            continue;


        int bx =
            object->x - camera_x;


        if (bx + object->width < 0 ||
            bx >= 240)
        {
            continue;
        }


        /*
            Base material.
        */

        gba_rect(
            bx,
            object->y,
            object->width,
            object->height,
            RGB15(10, 8, 8)
        );


        /*
            Different visual signatures for different
            breakable object types.
        */

        if (object->type == BREAKABLE_BARRIER)
        {
            gba_rect(
                bx + 3,
                object->y + 5,
                2,
                object->height - 10,
                RGB15(18, 3, 4)
            );

            gba_rect(
                bx + 7,
                object->y + 14,
                2,
                object->height - 18,
                RGB15(20, 3, 4)
            );
        }
        else if (object->type == BREAKABLE_WALL)
        {
            gba_rect(
                bx + 2,
                object->y + 3,
                object->width - 4,
                2,
                RGB15(15, 5, 5)
            );
        }
        else if (object->type == BREAKABLE_FLOOR)
        {
            gba_rect(
                bx + 2,
                object->y,
                object->width - 4,
                3,
                RGB15(18, 4, 4)
            );
        }
        else if (object->type == BREAKABLE_CEILING)
        {
            gba_rect(
                bx + 2,
                object->y + object->height - 3,
                object->width - 4,
                3,
                RGB15(18, 4, 4)
            );
        }
    }


    /*
        Secret HP upgrade.
    */

    if (game_state != 0 &&
        !game_state->secret_hp_01)
    {
        int sx =
            SECRET_HP_X - camera_x;


        if (sx + SECRET_HP_WIDTH >= 0 &&
            sx < 240)
        {
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
        Floor markers.
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
