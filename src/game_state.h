#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct
{
    /*
        Exploration / collectibles.
    */
    int secret_hp_01;

    /*
        Breakable world objects.

        0 = intact
        1 = destroyed
    */
    int barrier_01_destroyed;

    /*
        Future breakable objects.
        These are already part of the persistent state so
        the world can grow without redesigning the system.
    */
    int barrier_02_destroyed;
    int wall_01_destroyed;
    int floor_01_destroyed;
    int ceiling_01_destroyed;

    /*
        Shortcuts.
    */
    int shortcut_01_open;

    /*
        Abilities.
    */
    int ability_dash;
    int ability_wall_jump;

    /*
        Boss progression.
    */
    int boss_01_defeated;

} GameState;


void game_state_init(GameState *state);

#endif
