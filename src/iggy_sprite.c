#include "iggy_sprite.h"

/*
    NECROROCK — Iggy-inspired protagonist

    SOURCE SIZE:
        16 x 20 pixels

    GBA DISPLAY:
        32 x 40 pixels (2x scale)

    DESIGN:
        - small proportionate head
        - swept blonde hair
        - bare torso
        - lean/wiry body
        - slightly hunched punk posture
        - long legs
        - dark pants
        - heavy dark boots
        - red waist/accent

    PIXEL LEGEND:
        . = transparent
        Y = blonde hair
        S = skin
        s = skin shadow
        R = red accent
        J = pants
        B = boots

    IMPORTANT:
        Every row is exactly 16 pixels wide.
*/

const char iggy_idle[] =
    "....YYYYYY......"
    "....YYYYYYY....."
    "..YYYYYYYYYY...."
    "..YYSSSSSSYY...."
    "..YSSSSSSSSY...."
    "...YSSSSSSY....."
    "....SSSSSS......"
    "...SSSSSSSS....."
    "..SSSSRRSSSS...."
    "..SSSSSSSSSS...."
    "...SSSSSSSS....."
    "...SSssssSS....."
    "..SS......SS...."
    "..S........S...."
    "..JJ......JJ...."
    "..JJ......JJ...."
    "..JJJ....JJJ...."
    "..JJ.....JJ....."
    "..BBB....BBB...."
    ".BBBB....BBBB...";

const char iggy_walk_1[] =
    "....YYYYYY......"
    "....YYYYYYY....."
    "..YYYYYYYYYY...."
    "..YYSSSSSSYY...."
    "..YSSSSSSSSY...."
    "...YSSSSSSY....."
    "....SSSSSS......"
    "...SSSSSSSS....."
    "..SSSSRRSSSS...."
    "..SSSSSSSSSS...."
    "...SSSSSSSS....."
    "...SSssssSS....."
    "..SS......SS...."
    "..S.......SS...."
    "..JJ......JJ...."
    ".JJJ......JJ...."
    "JJJ.......JJ...."
    "JJ........JJ...."
    "BB........BBB..."
    "B.........BBBB..";

const char iggy_walk_2[] =
    "....YYYYYY......"
    "....YYYYYYY....."
    "..YYYYYYYYYY...."
    "..YYSSSSSSYY...."
    "..YSSSSSSSSY...."
    "...YSSSSSSY....."
    "....SSSSSS......"
    "...SSSSSSSS....."
    "..SSSSRRSSSS...."
    "..SSSSSSSSSS...."
    "...SSSSSSSS....."
    "...SSssssSS....."
    "..SS......SS...."
    "..SS.......S...."
    "...JJ.....JJ...."
    "...JJ....JJ....."
    "..JJJ...JJJ....."
    "..JJ....JJ......"
    ".BBB....BBB....."
    "BBBB....BBBB....";

const char iggy_attack[] =
    "....YYYYYY......"
    "....YYYYYYY....."
    "..YYYYYYYYYY...."
    "..YYSSSSSSYY...."
    "..YSSSSSSSSY...."
    "...YSSSSSSY....."
    "....SSSSSS......"
    "...SSSSSSSS....."
    ".SSSSSRRSSSSS..."
    "..SSSSSSSSSS...."
    "...SSSSSSSS....."
    "..SSssssSS......"
    "..SS......SS...."
    ".SS........SS..."
    "..JJ......JJ...."
    ".JJJ......JJ...."
    "JJJ........JJ..."
    "JJ..........JJ.."
    "BB..........BB.."
    "BBBB........BBBB";
