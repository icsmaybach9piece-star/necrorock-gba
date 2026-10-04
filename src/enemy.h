#ifndef ENEMY_H
#define ENEMY_H

typedef struct
{
    int x;
    int y;

    int width;
    int height;

    int velocity_x;

    int hp;
    int alive;

    int animation_frame;
    int animation_timer;
} Enemy;

void enemy_init(Enemy *enemy, int x, int y);

void enemy_update(
    Enemy *enemy,
    int player_x,
    int player_y
);

void enemy_draw(
    const Enemy *enemy,
    int camera_x
);

int enemy_hit(
    Enemy *enemy,
    int attack_x,
    int attack_y,
    int attack_width,
    int attack_height
);

#endif
