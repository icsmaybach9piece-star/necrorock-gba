#include <stdint.h>
#include "../include/gba.h"
#include "input.h"

#define REG_KEYINPUT (*(volatile uint16_t*)0x04000130)

static uint16_t current_keys = 0;
static uint16_t previous_keys = 0;

void input_update(void)
{
    previous_keys = current_keys;
    current_keys = (uint16_t)(~REG_KEYINPUT & 0x03FF);
}

uint16_t input_current(void)
{
    return current_keys;
}

uint16_t input_pressed(void)
{
    return current_keys & (uint16_t)~previous_keys;
}
