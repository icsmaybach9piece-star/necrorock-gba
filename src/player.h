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

    int animation_frame;
    int animation_timer;
} Player;

void player_init(Player *player);

void player_update(Player *player);

void player_draw(
    const Player *player,
    int camera_x
);

#endif
