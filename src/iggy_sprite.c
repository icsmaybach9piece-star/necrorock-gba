#include "iggy_sprite.h"

/*
    NECROROCK — Iggy-inspired protagonist

    Source: 16 x 20 pixels
    Display: 32 x 40 pixels

    Every row MUST contain exactly 16 characters.

    . = transparent
    S = skin
    s = skin shadow
    Y = blonde hair
    y = hair shadow
    J = pants
    B = boots
    R = red accent
*/

const char iggy_idle[] =
    ".....YYYYYY....."
    "....YYYYYYYY...."
    "....YYSSSSYY...."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYSSYY....."
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "....SSSSSS......"
    "...SS......SS..."
    "...S........SS.."
    "...JJ......JJ..."
    "..JJJ......JJ..."
    "..JJJ.....JJJ..."
    "..JJ......JJ...."
    "..BBB....BBB...."
    ".BBBB....BBBB...";

const char iggy_walk_1[] =
    ".....YYYYYY....."
    "....YYYYYYYY...."
    "....YYSSSSYY...."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYSSYY....."
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "....SSSSSS......"
    "...SS......SS..."
    "...S.......SS..."
    "..JJ.......JJ..."
    ".JJJ.......JJ..."
    "JJJ........JJ..."
    "JJ.........JJ..."
    "BB........BBB..."
    "B.........BBBB..";

const char iggy_walk_2[] =
    ".....YYYYYY....."
    "....YYYYYYYY...."
    "....YYSSSSYY...."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYSSYY....."
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "....SSSSSS......"
    "...SS......SS..."
    "...SS.......S..."
    "...JJ......JJ..."
    "...JJ.....JJ...."
    "..JJJ.....JJJ..."
    "..JJ.....JJ....."
    ".BBB.....BBB...."
    "BBBB.....BBBB...";

const char iggy_attack[] =
    ".....YYYYYY....."
    "....YYYYYYYY...."
    "....YYSSSSYY...."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYSSYY....."
    "......SSSS......"
    ".....SSSSSS....."
    "...SSSSRRSSSS..."
    "..SSSSSSSSSS...."
    "..sSSSSSSSSs...."
    "...SSSSSSSS....."
    "..SS......SS...."
    ".SS........SS..."
    "..JJ......JJ...."
    ".JJJ......JJ...."
    "JJJ........JJ..."
    "JJ..........JJ.."
    "BB..........BB.."
    "BBBB........BBBB";
