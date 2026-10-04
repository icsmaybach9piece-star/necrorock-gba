#include <stdint.h>

#include "../include/gba.h"

#include "enemy.h"
#include "world.h"

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

void enemy_init(
    Enemy *enemy,
    int x,
    int y
)
{
    enemy->x = x;
    enemy->y = y;

    enemy->width = 18;
    enemy->height = 18;

    enemy->velocity_x = 1;

    enemy->hp = 3;
    enemy->alive = 1;

    enemy->animation_frame = 0;
    enemy->animation_timer = 0;
}

void enemy_update(
    Enemy *enemy,
    int player_x,
    int player_y
)
{
    (void)player_x;
    (void)player_y;

    if (!enemy->alive)
        return;

    int next_x = enemy->x + enemy->velocity_x;

    /*
       Turn around if we hit something.
    */

    if (world_collides(
            next_x,
            enemy->y,
            enemy->width,
            enemy->height))
    {
        enemy->velocity_x = -enemy->velocity_x;
    }
    else
    {
        enemy->x = next_x;
    }

    /*
       Animation.
    */

    enemy->animation_timer++;

    if (enemy->animation_timer >= 12)
    {
        enemy->animation_timer = 0;
        enemy->animation_frame++;

        if (enemy->animation_frame >= 2)
            enemy->animation_frame = 0;
    }
}

int enemy_hit(
    Enemy *enemy,
    int attack_x,
    int attack_y,
    int attack_width,
    int attack_height
)
{
    if (!enemy->alive)
        return 0;

    if (!overlap(
            attack_x,
            attack_width,
            enemy->x,
            enemy->width))
    {
        return 0;
    }

    if (!overlap(
            attack_y,
            attack_height,
            enemy->y,
            enemy->height))
    {
        return 0;
    }

    enemy->hp--;

    if (enemy->hp <= 0)
    {
        enemy->alive = 0;
    }

    return 1;
}

void enemy_draw(
    const Enemy *enemy,
    int camera_x
)
{
    if (!enemy->alive)
        return;

    int x = enemy->x - camera_x;
    int y = enemy->y;

    /*
       Main body.
    */

    gba_rect(
        x,
        y + 4,
        enemy->width,
        12,
        RGB15(8, 9, 9)
    );

    /*
       Organic shell.
    */

    gba_rect(
        x + 3,
        y,
        12,
        8,
        RGB15(12, 12, 12)
    );

    /*
       Eyes.
    */

    gba_rect(
        x + 4,
        y + 5,
        3,
        3,
        RGB15(20, 2, 3)
    );

    gba_rect(
        x + 11,
        y + 5,
        3,
        3,
        RGB15(20, 2, 3)
    );

    /*
       Legs.
    */

    if (enemy->animation_frame == 0)
    {
        gba_rect(
            x + 2,
            y + 15,
            4,
            3,
            RGB15(5, 5, 5)
        );

        gba_rect(
            x + 12,
            y + 15,
            4,
            3,
            RGB15(5, 5, 5)
        );
    }
    else
    {
        gba_rect(
            x + 1,
            y + 14,
            4,
            4,
            RGB15(5, 5, 5)
        );

        gba_rect(
            x + 13,
            y + 14,
            4,
            4,
            RGB15(5, 5, 5)
        );
    }
}
