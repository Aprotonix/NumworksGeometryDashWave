#include "eadkpp.h"
#include "palette.h"
#pragma once



const EADK::Color player_frame_down[256] = {
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black
};

const EADK::Color player_frame_half_down[256] = {
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Void, Void, Black, Black, Black, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void, Void, Void,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Void, Void, Void, Void, Void, Void, Black, Black, Void, Void, Black, Black, Black, Void, Void, Void,
    Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void
};

const EADK::Color player_frame_middle[256] = {
    Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Void, Black, Black, Void, Void, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Black, Black, Black, Black, Void, Black, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void, Void,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Void, Black, Void, Void, Void,
    Void, Void, Void, Void, Black, Black, Black, Void, Black, Black, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Black, Black, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void
};

const EADK::Color player_frame_half_up[256] = {
    Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Void, Void, Black, Black, Void, Void, Black, Black, Black, Void, Void, Void,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Void, Void, Black, Black, Black, Void, Void, Void, Void, Void, Void, Void,
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Void, Void, Void, Void, Void, Void, Void
};

const EADK::Color player_frame_up[256] = {
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Void, Void, Void, Void, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void,
    Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Void, Void, Void, Void
};

static const EADK::Color* player_frames[5] = {
    player_frame_down,//x - 3, y + 3
    player_frame_half_down, // x -3, y + 2
    player_frame_middle, //x - 3, y + 0
    player_frame_half_up,// x - 3, y - 2
    player_frame_up// x - 3, y - 3

 //   player_fram_middle,
    
};