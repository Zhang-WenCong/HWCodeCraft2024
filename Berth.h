#pragma once

#include "common.h"


class Berth {
public:
    int berth_id;
    int x, y;
    int loading_speed;  // 装货时间 1 <= Velocity <= 5，即每帧可以装载的物品数

    int cur_goods_num = 0; // 拥有货物

    bool is_used = false;

    Berth() {}
     
    Berth(int id, int x, int y, int loading_speed) 
        :berth_id(id)
        ,x(x)
        ,y(y)
        ,loading_speed(loading_speed) {
    }

    // 是否在泊位内
    bool is_in_berth(int _x, int _y);

    int get_abs_len(int _x, int _y);

    void show();
    
};