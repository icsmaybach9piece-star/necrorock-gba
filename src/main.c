#include <stdint.h>

#include "../include/gba.h"

#include "input.h"
#include "player.h"
#include "world.h"

#define REG_DISPCNT (*(volatile uint16_t*)0x04000000)

#define MODE3       3
#define BG2_ENABLE  (1 << 10)

static void draw_title(void)
{
    gba_clear(RGB15(1, 1, 2));

    /*
       NECROROCK title placeholder.
       We'll replace this with proper tile artwork later.
    */

    gba_rect(
        42,
        35,
        156,
        8,
        RGB15(16, 3, 4)
    );

    gba_rect(
        60,
        50,
        120,
        5,
        RGB15(10, 10, 10)
    );

    gba_rect(
        84,
        100,
        72,
        4,
        RGB15(12, 12, 12)
    );
}

int main(void)
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    world_init();

    Player player;

    player_init(&player);

    int title = 1;

    while (1)
    {
        gba_vsync();

        input_update();

        if (title)
        {
            draw_title();

            if (input_pressed() & KEY_START)
            {
                title = 0;
            }

            continue;
        }

        player_update(&player);

        /*
           Camera follows the player.
        */

        int camera_x = player.x - 112;

        if (camera_x < 0)
            camera_x = 0;

        if (camera_x > WORLD_WIDTH - 240)
            camera_x = WORLD_WIDTH - 240;

        world_draw(camera_x);

        player_draw(
            &player,
            camera_x
        );
    }

    return 0;
}
