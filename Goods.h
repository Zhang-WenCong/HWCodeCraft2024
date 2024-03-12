#pragma once

#include "common.h"

class Goods {
public:
    int x, y;
    int value;
    int gen_frame; // 生成时的帧数，过1000帧会消失

    int goods_id;
    int assigned_robot_id; // 分配的robot id，-1表示未分配 

    Goods() {}
    
    Goods(int x, int y, int v, int f, int id) : x(x), y(y), value(v), gen_frame(f), goods_id(id), assigned_robot_id(-1) {}

};