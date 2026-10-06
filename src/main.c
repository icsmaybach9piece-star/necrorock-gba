#include <stdint.h>

#include "../include/gba.h"

#include "input.h"
#include "player.h"
#include "world.h"
#include "enemy.h"
#include "game_state.h"

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


static void draw_hud(
    const Player *player,
    const GameState *state
)
{
    /*
        HP container.
    */

    gba_rect(
        8,
        8,
        64,
        8,
        RGB15(3, 3, 3)
    );


    /*
        HP bar.
    */

    int hp_width = player->hp * 10;

    if (hp_width > 60)
        hp_width = 60;

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


    /*
        Tiny exploration indicator.

        This is temporary and will become
        proper HUD art later.
    */

    if (state->secret_hp_01)
    {
        gba_rect(
            78,
            8,
            8,
            8,
            RGB15(25, 4, 5)
        );
    }
}


int main(void)
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;


    /*
        ======================================================
        GAME STATE
        ======================================================
    */

    GameState game_state;

    game_state_init(
        &game_state
    );


    /*
        ======================================================
        WORLD
        ======================================================
    */

    world_init(
        &game_state
    );


    /*
        ======================================================
        PLAYER / ENEMY
        ======================================================
    */

    Player player;
    Enemy enemy;

    player_init(
        &player
    );

    enemy_init(
        &enemy,
        210,
        118
    );


    int title = 1;


    /*
        ======================================================
        MAIN LOOP
        ======================================================
    */

    while (1)
    {
        gba_vsync();

        input_update();


        /*
            TITLE
        */

        if (title)
        {
            draw_title();

            if (input_pressed() & KEY_START)
                title = 0;

            continue;
        }


        /*
            PLAYER
        */

        player_update(
            &player
        );


        /*
            WORLD INTERACTION
        */

        world_update(
            player.x,
            player.y,
            player.width,
            player.height,

            player_is_attacking(
                &player
            ),

            player_attack_x(
                &player
            ),

            player_attack_y(
                &player
            ),

            player_attack_width(
                &player
            ),

            player_attack_height(
                &player
            )
        );


        /*
            SECRET HP UPGRADE

            For now the upgrade increases maximum
            practical HP immediately.

            Later we'll create a proper maximum-HP
            system instead of this simple prototype.
        */

        static int previous_secret = 0;

        if (game_state.secret_hp_01 &&
            !previous_secret)
        {
            player.hp += 1;

            if (player.hp > 6)
                player.hp = 6;
        }

        previous_secret =
            game_state.secret_hp_01;


        /*
            ENEMY
        */

        enemy_update(
            &enemy,
            player.x,
            player.y
        );


        /*
            COMBAT
        */

        if (player_is_attacking(&player))
        {
            enemy_hit(
                &enemy,

                player_attack_x(
                    &player
                ),

                player_attack_y(
                    &player
                ),

                player_attack_width(
                    &player
                ),

                player_attack_height(
                    &player
                )
            );
        }


        /*
            CAMERA
        */

        int camera_x =
            player.x - 112;

        if (camera_x < 0)
            camera_x = 0;

        if (camera_x >
            WORLD_WIDTH - 240)
        {
            camera_x =
                WORLD_WIDTH - 240;
        }


        /*
            DRAW WORLD
        */

        world_draw(
            camera_x
        );


        /*
            DRAW ENEMY
        */

        enemy_draw(
            &enemy,
            camera_x
        );


        /*
            DRAW PLAYER
        */

        player_draw(
            &player,
            camera_x
        );


        /*
            HUD
        */

        draw_hud(
            &player,
            &game_state
        );
    }


    return 0;
}
