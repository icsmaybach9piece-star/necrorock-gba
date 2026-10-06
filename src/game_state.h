#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct
{
    /*
        Permanent progression flags.

        0 = not obtained
        1 = obtained
    */

    int secret_hp_01;
    int barrier_01_destroyed;
    int shortcut_01_open;

    int ability_dash;
    int ability_wall_jump;

    int boss_01_defeated;
} GameState;

void game_state_init(GameState *state);

#endif
