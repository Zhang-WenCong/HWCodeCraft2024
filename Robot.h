#pragma once

#include <list>
#include "common.h"

using namespace std;

class Robot {
public:
    int robot_id;
    int x, y, goods;
    int status;    

    bool is_valid = true; // 是否是无效机器人
    
    int target_goods_id = -1;
    int target_berth_id = -1;

    int task_type = -1;     // -1未分配任务， 0表示取货， 1表示取到货物后送到泊位
    list<int> path_to_goods;  // 到货物路径
    list<int> path_to_berth;  // 到泊位路径

    Robot(int id, int startX, int startY) : robot_id(id), x(startX), y(startY) {
        status = 1;
        goods = 0;
    }

    // 是否空闲
    bool is_idle();

    void show();

};