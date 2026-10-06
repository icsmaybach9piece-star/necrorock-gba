#include <stdint.h>

#include "../include/gba.h"

/*
    NECROROCK GBA RENDERER
    Mode 4: 240 x 160, 8-bit indexed color.

    Page 0: 0x06000000
    Page 1: 0x0600A000

    Draw on the hidden page, then flip at VBlank.
*/

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160
#define SCREEN_PIXELS (SCREEN_WIDTH * SCREEN_HEIGHT)

#define PAGE0_ADDRESS 0x06000000
#define PAGE1_ADDRESS 0x0600A000

#define PALETTE_ADDRESS 0x05000000
#define PALETTE_SIZE 256

static volatile uint16_t *const palette =
    (volatile uint16_t*)PALETTE_ADDRESS;

static volatile uint16_t *const display_control =
    (volatile uint16_t*)0x04000000;

static volatile uint16_t *const scanline =
    (volatile uint16_t*)0x04000006;

static uint16_t known_colors[PALETTE_SIZE];
static unsigned int known_color_count = 0;

/*
    The display starts on page 0.
    We draw the first frame on page 1.
*/

static int draw_page = 1;

static volatile uint8_t *backbuffer(void)
{
    if (draw_page)
    {
        return (volatile uint8_t*)PAGE1_ADDRESS;
    }

    return (volatile uint8_t*)PAGE0_ADDRESS;
}

/*
    Convert an RGB15 color into a palette index.

    Colors are cached so each distinct color receives
    one stable palette entry across all source files.
*/

static uint8_t color_index(uint16_t color)
{
    for (unsigned int i = 0; i < known_color_count; ++i)
    {
        if (known_colors[i] == color)
        {
            return (uint8_t)i;
        }
    }

    /*
        Allocate a palette entry for a new color.
    */

    if (known_color_count < PALETTE_SIZE)
    {
        unsigned int index = known_color_count++;

        known_colors[index] = color;
        palette[index] = color;

        return (uint8_t)index;
    }

    /*
        Safety fallback: find the closest existing color.
        This should rarely be needed for our prototype.
    */

    unsigned int best_index = 0;
    unsigned int best_distance = 0xFFFFFFFFu;

    int red = color & 31;
    int green = (color >> 5) & 31;
    int blue = (color >> 10) & 31;

    for (unsigned int i = 0; i < PALETTE_SIZE; ++i)
    {
        int pr = known_colors[i] & 31;
        int pg = (known_colors[i] >> 5) & 31;
        int pb = (known_colors[i] >> 10) & 31;

        int dr = red - pr;
        int dg = green - pg;
        int db = blue - pb;

        unsigned int distance =
            (unsigned int)(dr * dr + dg * dg + db * db);

        if (distance < best_distance)
        {
            best_distance = distance;
            best_index = i;
        }
    }

    return (uint8_t)best_index;
}

/*
    Initialize Mode 4 and the palette cache.
*/

void gba_init(void)
{
    known_color_count = 0;
    draw_page = 1;

    for (unsigned int i = 0; i < PALETTE_SIZE; ++i)
    {
        known_colors[i] = 0;
        palette[i] = 0;
    }

    /*
        Enable Mode 4, background 2, visible page 0.
    */

    REG_DISPCNT = MODE4 | BG2_ENABLE;

    /*
        Clear both pages.
    */

    volatile uint8_t *page0 =
        (volatile uint8_t*)PAGE0_ADDRESS;

    volatile uint8_t *page1 =
        (volatile uint8_t*)PAGE1_ADDRESS;

    for (unsigned int i = 0; i < SCREEN_PIXELS; ++i)
    {
        page0[i] = 0;
        page1[i] = 0;
    }
}

/*
    Wait for the next VBlank and display the finished page.
*/

void gba_flip(void)
{
    /*
        First ensure we're outside the previous VBlank.
    */

    while (*scanline >= 160)
    {
    }

    /*
        Wait until the next VBlank begins.
    */

    while (*scanline < 160)
    {
    }

    /*
        Select the page we just finished drawing.
    */

    uint16_t control = *display_control;

    if (draw_page)
    {
        control |= PAGE_SELECT;
    }
    else
    {
        control &= (uint16_t)~PAGE_SELECT;
    }

    *display_control = control;

    /*
        The old visible page is now our drawing page.
    */

    draw_page = !draw_page;
}

/*
    Clear the hidden framebuffer.
*/

void gba_clear(uint16_t color)
{
    volatile uint8_t *buffer = backbuffer();
    uint8_t index = color_index(color);

    for (unsigned int i = 0; i < SCREEN_PIXELS; ++i)
    {
        buffer[i] = index;
    }
}

/*
    Draw one pixel.
*/

void gba_pixel(
    int x,
    int y,
    uint16_t color
)
{
    if (x < 0 || x >= SCREEN_WIDTH ||
        y < 0 || y >= SCREEN_HEIGHT)
    {
        return;
    }

    volatile uint8_t *buffer = backbuffer();
    uint8_t index = color_index(color);

    buffer[y * SCREEN_WIDTH + x] = index;
}

/*
    Draw a clipped rectangle.

    Convert the color once per rectangle instead of
    performing palette lookup for every pixel.
*/

void gba_rect(
    int x,
    int y,
    int width,
    int height,
    uint16_t color
)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    int x0 = x;
    int y0 = y;
    int x1 = x + width;
    int y1 = y + height;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;

    if (x1 > SCREEN_WIDTH) x1 = SCREEN_WIDTH;
    if (y1 > SCREEN_HEIGHT) y1 = SCREEN_HEIGHT;

    if (x0 >= x1 || y0 >= y1)
    {
        return;
    }

    volatile uint8_t *buffer = backbuffer();
    uint8_t index = color_index(color);

    for (int yy = y0; yy < y1; ++yy)
    {
        unsigned int offset = yy * SCREEN_WIDTH + x0;

        for (int xx = x0; xx < x1; ++xx)
        {
            buffer[offset++] = index;
        }
    }
}
