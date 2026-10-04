#include <stdint.h>

#include "../include/gba.h"

#include "input.h"
#include "player.h"
#include "world.h"
#include "enemy.h"

#define REG_DISPCNT (*(volatile uint16_t*)0x04000000)

#define MODE3       3
#define BG2_ENABLE  (1 << 10)

static void draw_title(void)
{
    gba_clear(RGB15(1, 1, 2));

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

static void draw_hud(const Player *player)
{
    /*
       HP background.
    */

    gba_rect(
        8,
        8,
        54,
        8,
        RGB15(3, 3, 3)
    );

    /*
       HP.
    */

    int hp_width = player->hp * 10;

    if (hp_width > 50)
        hp_width = 50;

    if (hp_width > 0)
    {
        gba_rect(
            10,
            10,
            hp_width,
            4,
            RGB15(20, 3, 4)
        );
    }
}

int main(void)
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    world_init();

    Player player;
    Enemy enemy;

    player_init(&player);

    /*
       First enemy in the Necro-Chapel.
    */

    enemy_init(
        &enemy,
        210,
        118
    );

    int title = 1;

    while (1)
    {
        gba_vsync();

        input_update();

        if (title)
        {
            draw_title();

            if (input_pressed() & KEY_START)
                title = 0;

            continue;
        }

        player_update(&player);

        enemy_update(
            &enemy,
            player.x,
            player.y
        );

        /*
           Player attack collision.
        */

        if (player_is_attacking(&player))
        {
            enemy_hit(
                &enemy,
                player_attack_x(&player),
                player_attack_y(&player),
                player_attack_width(&player),
                player_attack_height(&player)
            );
        }

        /*
           Camera follows player.
        */

        int camera_x = player.x - 112;

        if (camera_x < 0)
            camera_x = 0;

        if (camera_x > WORLD_WIDTH - 240)
            camera_x = WORLD_WIDTH - 240;

        world_draw(camera_x);

        enemy_draw(
            &enemy,
            camera_x
        );

        player_draw(
            &player,
            camera_x
        );

        draw_hud(&player);
    }

    return 0;
}
