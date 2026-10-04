#include <stdint.h>
#include "../include/gba.h"

#include "player.h"
#include "input.h"
#include "world.h"

#define GRAVITY       1
#define MAX_FALL_SPEED 6

#define RUN_SPEED     2
#define JUMP_SPEED   -8

static void move_horizontal(Player *p)
{
    int next_x = p->x + p->velocity_x;

    if (!world_collides(
            next_x,
            p->y,
            p->width,
            p->height))
    {
        p->x = next_x;
    }
    else
    {
        p->velocity_x = 0;
    }
}

static void move_vertical(Player *p)
{
    int next_y = p->y + p->velocity_y;

    if (!world_collides(
            p->x,
            next_y,
            p->width,
            p->height))
    {
        p->y = next_y;
        p->grounded = 0;
        return;
    }

    if (p->velocity_y > 0)
    {
        /*
           Falling onto a surface.
        */

        while (world_collides(
            p->x,
            p->y + 1,
            p->width,
            p->height))
        {
            p->y--;
        }

        p->grounded = 1;
    }

    p->velocity_y = 0;
}

void player_init(Player *player)
{
    player->x = 40;
    player->y = 80;

    player->width = 16;
    player->height = 28;

    player->velocity_x = 0;
    player->velocity_y = 0;

    player->grounded = 0;

    player->animation_frame = 0;
    player->animation_timer = 0;
}

void player_update(Player *p)
{
    uint16_t keys = input_current();
    uint16_t pressed = input_pressed();

    /*
       Horizontal movement
    */

    p->velocity_x = 0;

    if (keys & KEY_LEFT)
        p->velocity_x = -RUN_SPEED;

    if (keys & KEY_RIGHT)
        p->velocity_x = RUN_SPEED;

    /*
       Jump
    */

    if ((pressed & KEY_A) && p->grounded)
    {
        p->velocity_y = JUMP_SPEED;
        p->grounded = 0;
    }

    /*
       Gravity
    */

    if (p->velocity_y < MAX_FALL_SPEED)
        p->velocity_y += GRAVITY;

    move_horizontal(p);
    move_vertical(p);

    /*
       Animation timer
    */

    if (p->velocity_x != 0)
    {
        p->animation_timer++;

        if (p->animation_timer >= 8)
        {
            p->animation_timer = 0;
            p->animation_frame++;

            if (p->animation_frame >= 4)
                p->animation_frame = 0;
        }
    }
    else
    {
        p->animation_frame = 0;
        p->animation_timer = 0;
    }
}

void player_draw(
    const Player *p,
    int camera_x
)
{
    int x = p->x - camera_x;
    int y = p->y;

    uint16_t skin   = RGB15(20, 8, 5);
    uint16_t jacket = RGB15(7, 7, 7);
    uint16_t hair   = RGB15(4, 3, 3);
    uint16_t boot   = RGB15(2, 2, 2);
    uint16_t red    = RGB15(18, 3, 4);

    /*
       Head
    */

    gba_rect(
        x + 4,
        y,
        9,
        8,
        skin
    );

    /*
       Hair
    */

    gba_rect(
        x + 2,
        y - 2,
        13,
        4,
        hair
    );

    gba_rect(
        x + 1,
        y + 1,
        4,
        8,
        hair
    );

    /*
       Jacket/body
    */

    gba_rect(
        x + 2,
        y + 8,
        13,
        13,
        jacket
    );

    /*
       Shirt detail
    */

    gba_rect(
        x + 7,
        y + 9,
        3,
        8,
        red
    );

    /*
       Arms
    */

    gba_rect(
        x,
        y + 10,
        3,
        11,
        jacket
    );

    gba_rect(
        x + 15,
        y + 10,
        3,
        11,
        jacket
    );

    /*
       Legs
    */

    int leg_offset = 0;

    if (p->velocity_x != 0)
    {
        if (p->animation_frame == 1)
            leg_offset = 2;

        if (p->animation_frame == 3)
            leg_offset = -2;
    }

    gba_rect(
        x + 4 + leg_offset,
        y + 21,
        4,
        7,
        jacket
    );

    gba_rect(
        x + 11 - leg_offset,
        y + 21,
        4,
        7,
        jacket
    );

    /*
       Boots
    */

    gba_rect(
        x + 3 + leg_offset,
        y + 27,
        6,
        2,
        boot
    );

    gba_rect(
        x + 10 - leg_offset,
        y + 27,
        6,
        2,
        boot
    );
}
