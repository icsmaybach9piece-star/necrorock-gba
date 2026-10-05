#include <stdint.h>

#include "../include/gba.h"

#include "player.h"
#include "input.h"
#include "world.h"
#include "sprite.h"
#include "iggy_sprite.h"

#define GRAVITY         1
#define MAX_FALL_SPEED  6

#define RUN_SPEED       2
#define JUMP_SPEED     -8

#define ATTACK_DURATION 10
#define ATTACK_COOLDOWN 14

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

    player->facing = 1;

    player->attacking = 0;
    player->attack_timer = 0;
    player->attack_cooldown = 0;

    player->hp = 5;
    player->invulnerability_timer = 0;

    player->animation_frame = 0;
    player->animation_timer = 0;
}

void player_update(Player *p)
{
    uint16_t keys = input_current();
    uint16_t pressed = input_pressed();

    /*
       Attack.
    */

    if ((pressed & KEY_B) &&
        p->attack_cooldown == 0)
    {
        p->attacking = 1;
        p->attack_timer = ATTACK_DURATION;
        p->attack_cooldown = ATTACK_COOLDOWN;
    }

    if (p->attack_timer > 0)
    {
        p->attack_timer--;

        if (p->attack_timer == 0)
            p->attacking = 0;
    }

    if (p->attack_cooldown > 0)
        p->attack_cooldown--;

    /*
       Horizontal movement.
    */

    p->velocity_x = 0;

    if (keys & KEY_LEFT)
    {
        p->velocity_x = -RUN_SPEED;
        p->facing = -1;
    }

    if (keys & KEY_RIGHT)
    {
        p->velocity_x = RUN_SPEED;
        p->facing = 1;
    }

    /*
       Jump.
    */

    if ((pressed & KEY_A) && p->grounded)
    {
        p->velocity_y = JUMP_SPEED;
        p->grounded = 0;
    }

    /*
       Gravity.
    */

    if (p->velocity_y < MAX_FALL_SPEED)
        p->velocity_y += GRAVITY;

    move_horizontal(p);
    move_vertical(p);

    /*
       Animation.
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

    if (p->invulnerability_timer > 0)
        p->invulnerability_timer--;
}

int player_is_attacking(const Player *p)
{
    return p->attacking;
}

int player_attack_x(const Player *p)
{
    if (p->facing > 0)
        return p->x + p->width;

    return p->x - 20;
}

int player_attack_y(const Player *p)
{
    return p->y + 7;
}

int player_attack_width(const Player *p)
{
    return 20;
}

int player_attack_height(const Player *p)
{
    return 14;
}

void player_draw(
    const Player *p,
    int camera_x
)
{
    int x = p->x - camera_x;
    int y = p->y - 2;

    const uint8_t *sprite;

    /*
       Select animation frame.
    */

    if (p->attacking)
    {
        sprite = iggy_attack;
    }
    else if (p->velocity_x == 0)
    {
        sprite = iggy_idle;
    }
    else if (p->animation_frame & 1)
    {
        sprite = iggy_walk_1;
    }
    else
    {
        sprite = iggy_walk_2;
    }

    /*
       Draw the 16x16 source sprite at 2x.
    */

    sprite_draw(
        sprite,
        16,
        16,
        x - 8,
        y,
        2
    );

    /*
       Attack effect.
    */

    if (p->attacking)
    {
        int slash_x;

        if (p->facing > 0)
            slash_x = x + 24;
        else
            slash_x = x - 24;

        gba_rect(
            slash_x,
            y + 14,
            8,
            2,
            RGB15(25, 23, 18)
        );
    }
}
