#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    int x;
    int y;

    int width;
    int height;

    int velocity_x;
    int velocity_y;

    int grounded;

    int facing;

    int attacking;
    int attack_timer;
    int attack_cooldown;

    /* Attack direction: -1, 0, +1 on each axis. */
    int attack_dir_x;
    int attack_dir_y;

    int hp;
    int invulnerability_timer;

    int animation_frame;
    int animation_timer;
} Player;

void player_init(Player *player);

void player_update(Player *player);

void player_draw(
    const Player *player,
    int camera_x
);

int player_is_attacking(const Player *player);

int player_attack_x(const Player *player);

int player_attack_y(const Player *player);

int player_attack_width(const Player *player);

int player_attack_height(const Player *player);

#endif
