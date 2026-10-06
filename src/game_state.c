#include "game_state.h"


void game_state_init(GameState *state)
{
    /*
        Collectibles.
    */
    state->secret_hp_01 = 0;


    /*
        Breakable objects.
    */
    state->barrier_01_destroyed = 0;
    state->barrier_02_destroyed = 0;
    state->wall_01_destroyed = 0;
    state->floor_01_destroyed = 0;
    state->ceiling_01_destroyed = 0;


    /*
        Shortcuts.
    */
    state->shortcut_01_open = 0;


    /*
        Abilities.
    */
    state->ability_dash = 0;
    state->ability_wall_jump = 0;


    /*
        Bosses.
    */
    state->boss_01_defeated = 0;
}
