#ifndef GBA_H
#define GBA_H

#include <stdint.h>

#define REG_DISPCNT (*(volatile uint16_t*)0x04000000)
#define REG_VCOUNT  (*(volatile uint16_t*)0x04000006)

#define MODE4       4
#define BG2_ENABLE  (1 << 10)
#define PAGE_SELECT (1 << 4)

#define RGB15(r,g,b) \
    ((uint16_t)(((r) & 31) | (((g) & 31) << 5) | (((b) & 31) << 10)))

void gba_init(void);
void gba_flip(void);
void gba_clear(uint16_t color);

void gba_pixel(
    int x,
    int y,
    uint16_t color
);

void gba_rect(
    int x,
    int y,
    int width,
    int height,
    uint16_t color
);

#endif
