#include "iggy_sprite.h"

/*
    NECROROCK — Iggy-inspired protagonist

    Source sprite: 16 x 20
    Display size: 32 x 40

    . = transparent
    S = skin
    s = skin shadow
    Y = blonde hair
    y = hair shadow
    J = dark pants
    j = pants highlight
    B = boots
    R = red accent
    W = highlight

    Design:
    - compact head
    - swept blonde hair
    - bare torso
    - narrow waist
    - wiry arms
    - long legs
    - dark trousers
    - heavy boots
*/

const char iggy_idle[] =
    "...yyyyYYYYy..."
    "..yyYYYYYYYYyy.."
    "..yYYSSSSSYYy.."
    ".yYSSSSSSSSSYy."
    ".yYSSSSSSSSSYy."
    "..yyYSSSSYyy..."
    "...SSSSSSSS....."
    "..SSSSSSSSSS...."
    "..SSSSRRSSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "...SSSSSSSS....."
    "..SS......SS...."
    "..S........S...."
    ".JJ........JJ..."
    ".JJ........JJ..."
    ".JJ........JJ..."
    ".JJJ......JJJ..."
    "BBBB......BBBB.."
    "BBBB......BBBB..";

const char iggy_walk_1[] =
    "...yyyyYYYYy..."
    "..yyYYYYYYYYyy.."
    "..yYYSSSSSYYy.."
    ".yYSSSSSSSSSYy."
    ".yYSSSSSSSSSYy."
    "..yyYSSSSYyy..."
    "...SSSSSSSS....."
    "..SSSSSSSSSS...."
    "..SSSSRRSSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "...SSSSSSSS....."
    "..SS......SS...."
    "..S.......SS...."
    ".JJ.......JJ...."
    "JJJ.......JJ...."
    "JJ........JJJ..."
    "JJ........JJ...."
    "BB........BBB..."
    "B.........BBBB..";

const char iggy_walk_2[] =
    "...yyyyYYYYy..."
    "..yyYYYYYYYYyy.."
    "..yYYSSSSSYYy.."
    ".yYSSSSSSSSSYy."
    ".yYSSSSSSSSSYy."
    "..yyYSSSSYyy..."
    "...SSSSSSSS....."
    "..SSSSSSSSSS...."
    "..SSSSRRSSSS...."
    "...SSSSSSSS....."
    "...sSSSSSSs....."
    "...SSSSSSSS....."
    "..SS......SS...."
    "..SS.......S...."
    "...JJ......JJ..."
    "...JJ.....JJ...."
    "..JJJ....JJJ..."
    "..JJ.....JJ...."
    ".BBB.....BBB..."
    "BBBB.....BBBB..";

const char iggy_attack[] =
    "...yyyyYYYYy..."
    "..yyYYYYYYYYyy.."
    "..yYYSSSSSYYy.."
    ".yYSSSSSSSSSYy."
    ".yYSSSSSSSSSYy."
    "..yyYSSSSYyy..."
    "...SSSSSSSS....."
    "..SSSSSSSSSS...."
    ".SSSSSRRSSSSS..."
    ".SSSSSSSSSSSS..."
    "..sSSSSSSSSs...."
    "...SSSSSSSS....."
    "..SS......SS...."
    ".SS........SS..."
    ".JJ........JJ..."
    "JJJ........JJ..."
    "JJ..........JJ.."
    "JJ..........JJ.."
    "BB..........BB.."
    "BBBB........BBBB";
