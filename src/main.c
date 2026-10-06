#include <stdint.h>

#include "../include/gba.h"

#include "input.h"
#include "player.h"
#include "world.h"
#include "enemy.h"
#include "game_state.h"

static void draw_title(void)
{
    gba_clear(RGB15(1, 1, 2));

    gba_rect(
        42, 35, 156, 8,
        RGB15(16, 3, 4)
    );

    gba_rect(
        60, 50, 120, 5,
        RGB15(10, 10, 10)
    );

    gba_rect(
        84, 100, 72, 4,
        RGB15(12, 12, 12)
    );
}

static void draw_hud(
    const Player *player,
    const GameState *state
)
{
    gba_rect(
        8, 8, 64, 8,
        RGB15(3, 3, 3)
    );

    int hp_width = player->hp * 10;

    if (hp_width > 60)
        hp_width = 60;

    if (hp_width > 0)
    {
        gba_rect(
            10, 10, hp_width, 4,
            RGB15(20, 3, 4)
        );
    }

    if (state->secret_hp_01)
    {
        gba_rect(
            78, 8, 8, 8,
            RGB15(25, 4, 5)
        );
    }

    /*
        Second HP upgrade indicator.

        wall_01_destroyed is currently used by the prototype
        exploration system as the collected-state flag for the
        hidden alcove reward.
    */
    if (state->wall_01_destroyed)
    {
        gba_rect(
            90, 8, 8, 8,
            RGB15(25, 4, 5)
        );
    }
}

int main(void)
{
    gba_init();

    GameState game_state;
    game_state_init(&game_state);

    world_init(&game_state);

    Player player;
    Enemy enemy;

    player_init(&player);

    enemy_init(
        &enemy,
        210,
        118
    );

    int title = 1;
    int previous_secret = 0;
    int previous_secret_02 = 0;

    while (1)
    {
        input_update();

        if (title)
        {
            draw_title();

            gba_flip();

            if (input_pressed() & KEY_START)
            {
                title = 0;
            }

            continue;
        }

        player_update(&player);

        world_update(
            player.x,
            player.y,
            player.width,
            player.height,

            player_is_attacking(&player),

            player_attack_x(&player),
            player_attack_y(&player),
            player_attack_width(&player),
            player_attack_height(&player)
        );

        if (game_state.secret_hp_01 &&
            !previous_secret)
        {
            player.hp += 1;

            if (player.hp > 6)
                player.hp = 6;
        }

        /*
            The first hidden-room reward uses the prototype
            wall_01_destroyed state as its collected flag.
        */
        if (game_state.wall_01_destroyed &&
            !previous_secret_02)
        {
            player.hp += 1;

            if (player.hp > 6)
                player.hp = 6;
        }

        previous_secret = game_state.secret_hp_01;
        previous_secret_02 = game_state.wall_01_destroyed;

        enemy_update(
            &enemy,
            player.x,
            player.y
        );

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

        draw_hud(
            &player,
            &game_state
        );

        gba_flip();
    }

    return 0;
}
