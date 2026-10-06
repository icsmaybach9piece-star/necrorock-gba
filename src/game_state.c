#include "game_state.h"

void game_state_init(GameState *state)
{
    state->secret_hp_01 = 0;
    state->barrier_01_destroyed = 0;
    state->shortcut_01_open = 0;

    state->ability_dash = 0;
    state->ability_wall_jump = 0;

    state->boss_01_defeated = 0;
}
