#ifndef GBA_H
#define GBA_H

#include <stdint.h>

#define REG_DISPCNT (*(volatile uint16_t*)0x04000000)

#define VRAM ((volatile uint16_t*)0x06000000)

#define MODE3 3
#define BG2_ENABLE (1 << 10)

#define RGB15(r,g,b) \
    ((uint16_t)(((r) & 31) | (((g) & 31) << 5) | (((b) & 31) << 10)))

static inline void gba_vsync(void)
{
    volatile uint16_t *vcount = (volatile uint16_t*)0x04000006;

    while (*vcount >= 160);
    while (*vcount < 160);
}

static inline void gba_clear(uint16_t color)
{
    for (int i = 0; i < 240 * 160; ++i)
        VRAM[i] = color;
}

static inline void gba_pixel(int x, int y, uint16_t color)
{
    if (x < 0 || x >= 240 || y < 0 || y >= 160)
        return;

    VRAM[y * 240 + x] = color;
}

static inline void gba_rect(
    int x,
    int y,
    int width,
    int height,
    uint16_t color
)
{
    for (int yy = 0; yy < height; ++yy)
    {
        for (int xx = 0; xx < width; ++xx)
        {
            gba_pixel(x + xx, y + yy, color);
        }
    }
}

#endif
