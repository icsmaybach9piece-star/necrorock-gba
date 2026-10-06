#include <stdint.h>

#include "../include/gba.h"

#include "player.h"
#include "input.h"
#include "world.h"
#include "sprite.h"
#include "iggy_sprite.h"

#define GRAVITY          1
#define MAX_FALL_SPEED   6

#define RUN_SPEED        2
#define JUMP_SPEED      -8

#define ATTACK_DURATION  10
#define ATTACK_COOLDOWN  14

static void move_horizontal(Player *p)
{
    int next_x =
        p->x + p->velocity_x;

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
    int next_y =
        p->y + p->velocity_y;

    int surface_y =
        world_vertical_collision(
            p->x,
            p->y,
            next_y,
            p->width,
            p->height,
            p->velocity_y
        );

    if (surface_y < 0)
    {
        p->y = next_y;
        p->grounded = 0;
        return;
    }

    if (p->velocity_y > 0)
    {
        p->y =
            surface_y - p->height;

        p->velocity_y = 0;
        p->grounded = 1;

        return;
    }

    if (p->velocity_y < 0)
    {
        p->y =
            surface_y;

        p->velocity_y = 0;
        p->grounded = 0;

        return;
    }

    p->velocity_y = 0;
}

static void set_attack_direction(Player *p, uint16_t keys)
{
    int dx = 0;
    int dy = 0;

    if (keys & KEY_LEFT)
        dx = -1;

    if (keys & KEY_RIGHT)
        dx = 1;

    if (keys & KEY_UP)
        dy = -1;

    if (keys & KEY_DOWN)
        dy = 1;

    /*
        If no direction is held, attack in the direction Iggy
        is facing. This preserves the original control scheme.
    */
    if (dx == 0 && dy == 0)
        dx = p->facing;

    p->attack_dir_x = dx;
    p->attack_dir_y = dy;

    if (dx != 0)
        p->facing = dx;
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

    player->attack_dir_x = 1;
    player->attack_dir_y = 0;

    player->hp = 5;
    player->invulnerability_timer = 0;

    player->animation_frame = 0;
    player->animation_timer = 0;
}

void player_update(Player *p)
{
    uint16_t keys =
        input_current();

    uint16_t pressed =
        input_pressed();

    if ((pressed & KEY_B) &&
        p->attack_cooldown == 0)
    {
        set_attack_direction(p, keys);

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

    if ((pressed & KEY_A) &&
        p->grounded)
    {
        p->velocity_y = JUMP_SPEED;
        p->grounded = 0;
    }

    if (p->velocity_y < MAX_FALL_SPEED)
        p->velocity_y += GRAVITY;

    move_horizontal(p);
    move_vertical(p);

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
    if (p->attack_dir_x < 0)
        return p->x - 20;

    if (p->attack_dir_x > 0)
        return p->x + p->width;

    /*
        Vertical attacks are centered on Iggy.
    */
    return p->x - 2;
}

int player_attack_y(const Player *p)
{
    if (p->attack_dir_y < 0)
        return p->y - 20;

    if (p->attack_dir_y > 0)
        return p->y + p->height;

    return p->y + 7;
}

int player_attack_width(const Player *p)
{
    if (p->attack_dir_x == 0)
        return 20;

    return 20;
}

int player_attack_height(const Player *p)
{
    if (p->attack_dir_y != 0)
        return 20;

    return 14;
}

void player_draw(
    const Player *p,
    int camera_x
)
{
    int screen_x =
        p->x - camera_x;

    int screen_y =
        p->y - 12;

    const char *sprite;

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

    sprite_draw(
        sprite,
        16,
        20,
        screen_x - 8,
        screen_y,
        2
    );

    if (p->attacking)
    {
        int slash_x;
        int slash_y;
        int slash_w = 8;
        int slash_h = 2;

        /*
            The current slash is deliberately simple, but its
            position now follows all eight attack directions.
            The sprite animation can be upgraded later without
            changing combat collision.
        */
        if (p->attack_dir_x > 0)
        {
            slash_x = screen_x + 24;
            slash_y = screen_y + 18;

            gba_rect(
                slash_x,
                slash_y,
                slash_w,
                slash_h,
                RGB15(31, 28, 20)
            );
        }
        else if (p->attack_dir_x < 0)
        {
            slash_x = screen_x - 32;
            slash_y = screen_y + 18;

            gba_rect(
                slash_x,
                slash_y,
                slash_w,
                slash_h,
                RGB15(31, 28, 20)
            );
        }
        else if (p->attack_dir_y < 0)
        {
            slash_x = screen_x + 8;
            slash_y = screen_y - 10;

            gba_rect(
                slash_x,
                slash_y,
                slash_h,
                slash_w,
                RGB15(31, 28, 20)
            );
        }
        else
        {
            slash_x = screen_x + 8;
            slash_y = screen_y + 42;

            gba_rect(
                slash_x,
                slash_y,
                slash_h,
                slash_w,
                RGB15(31, 28, 20)
            );
        }
    }
}
