#include "iggy_sprite.h"

/*
   NECROROCK — Iggy-inspired player

   Source size: 16 x 20
   Display size: 32 x 40

   . = transparent
   S = skin / bare torso
   Y = blonde hair
   J = dark pants
   B = boots
   R = red accent
   W = highlight

   Design priorities:
   - small head
   - blonde hair
   - bare torso
   - lean/wiry anatomy
   - long legs
   - dark pants and boots
*/

const char iggy_idle[] =
    "......YYYY......"
    ".....YYYYYY....."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYYYYY....."
    "......YYYY......"
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "....SSSSSSSS...."
    ".....SSSSSS....."
    "....SSSSSSSS...."
    "...SS......SS..."
    "...S........S..."
    "..JJ........JJ.."
    "..JJ........JJ.."
    "..JJ........JJ.."
    ".JJJ........JJJ."
    ".BBB........BBB."
    "BBBB........BBBB";

const char iggy_walk_1[] =
    "......YYYY......"
    ".....YYYYYY....."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYYYYY....."
    "......YYYY......"
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "....SSSSSSSS...."
    ".....SSSSSS....."
    "....SSSSSSSS...."
    "...SS......SS..."
    "...S.......SS..."
    "..JJ.......JJ..."
    ".JJJ.......JJ..."
    "JJJ.........JJ.."
    "JJ..........JJ.."
    "BB..........BBB."
    "B...........BBB.";

const char iggy_walk_2[] =
    "......YYYY......"
    ".....YYYYYY....."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYYYYY....."
    "......YYYY......"
    "......SSSS......"
    ".....SSSSSS....."
    "....SSSRRSSS...."
    "....SSSSSSSS...."
    ".....SSSSSS....."
    "....SSSSSSSS...."
    "...SS......SS..."
    "...SS.......S..."
    "...JJ......JJ..."
    "...JJ.....JJ..."
    "..JJJ....JJJ..."
    "..JJ.....JJ...."
    ".BBB.....BBB..."
    "BBBB.....BBBB..";

const char iggy_attack[] =
    "......YYYY......"
    ".....YYYYYY....."
    "....YSSSSSSY...."
    "....YSSSSSSY...."
    ".....YYYYYY....."
    "......YYYY......"
    "......SSSS......"
    ".....SSSSSS....."
    "...SSSSRRSSSS..."
    "...SSSSSSSSSS..."
    "....SSSSSSSS...."
    "....SSSSSSSS...."
    "...SS......SS..."
    "..SS........SS.."
    "..JJ........JJ.."
    ".JJJ........JJ.."
    "JJJ..........JJ."
    "JJ............JJ"
    "BB............BB"
    "B..............B";
