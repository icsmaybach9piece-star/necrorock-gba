#include <stdint.h>
#include "../include/gba.h"

static volatile uint16_t *REG_KEYINPUT =
    (volatile uint16_t*)0x04000130;

#define KEY_A      (1 << 0)
#define KEY_B      (1 << 1)
#define KEY_SELECT (1 << 2)
#define KEY_START  (1 << 3)

#define KEY_RIGHT  (1 << 4)
#define KEY_LEFT   (1 << 5)
#define KEY_UP     (1 << 6)
#define KEY_DOWN   (1 << 7)

#define KEY_R      (1 << 8)
#define KEY_L      (1 << 9)

static uint16_t keys(void)
{
    return (uint16_t)(~(*REG_KEYINPUT) & 0x03FF);
}

static void draw_player(int x, int y)
{
    uint16_t skin = RGB15(20, 8, 5);
    uint16_t jacket = RGB15(8, 8, 8);
    uint16_t hair = RGB15(5, 4, 3);

    /*
       Temporary pixel-art Iggy silhouette.
       We'll replace this with the finished sprite artwork.
    */

    gba_rect(x + 5, y, 10, 8, skin);
    gba_rect(x + 3, y + 8, 14, 18, jacket);
    gba_rect(x, y + 10, 5, 14, jacket);
    gba_rect(x + 15, y + 10, 5, 14, jacket);

    gba_rect(x + 5, y - 3, 10, 4, hair);

    gba_rect(x + 5, y + 26, 5, 8, jacket);
    gba_rect(x + 13, y + 26, 5, 8, jacket);
}

static void draw_title(void)
{
    gba_clear(RGB15(1, 1, 1));

    /*
       Temporary title graphic.
       Proper tile-based title art comes later.
    */

    gba_rect(48, 35, 144, 8, RGB15(15, 4, 4));
    gba_rect(58, 50, 124, 4, RGB15(10, 10, 10));

    gba_rect(90, 100, 60, 4, RGB15(12, 12, 12));
}

static void draw_game(int player_x, int player_y)
{
    gba_clear(RGB15(1, 1, 2));

    /* Alien biomechanical floor */

    gba_rect(0, 128, 240, 32, RGB15(5, 6, 7));

    for (int x = 0; x < 240; x += 20)
    {
        gba_rect(x, 124, 3, 4, RGB15(10, 12, 12));
    }

    /* Organic structures */

    gba_rect(15, 60, 12, 68, RGB15(6, 7, 8));
    gba_rect(205, 35, 18, 93, RGB15(6, 7, 8));

    gba_rect(30, 78, 45, 8, RGB15(9, 10, 10));
    gba_rect(160, 70, 42, 7, RGB15(9, 10, 10));

    /* Strange biomechanical "veins" */

    gba_rect(27, 62, 3, 66, RGB15(13, 3, 4));
    gba_rect(203, 43, 3, 85, RGB15(13, 3, 4));

    draw_player(player_x, player_y);
}

int main(void)
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    int player_x = 110;
    int player_y = 94;

    int title = 1;

    while (1)
    {
        uint16_t k = keys();

        gba_vsync();

        if (title)
        {
            draw_title();

            if (k & KEY_START)
                title = 0;
        }
        else
        {
            if (k & KEY_LEFT)
                player_x--;

            if (k & KEY_RIGHT)
                player_x++;

            if (k & KEY_UP)
                player_y--;

            if (k & KEY_DOWN)
                player_y++;

            if (player_x < 0)
                player_x = 0;

            if (player_x > 220)
                player_x = 220;

            if (player_y < 0)
                player_y = 0;

            if (player_y > 120)
                player_y = 120;

            draw_game(player_x, player_y);
        }
    }

    return 0;
}
