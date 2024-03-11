#pragma once

#include <list>

using namespace std;

class Robot {
public:
    int robot_id;
    int x, y, goods;
    int status;    
    int target_x, target_y; // 目标坐标

    bool in_task = false;        // 是否分配任务
    list<pair<int, int>> path;  // 到目标（货物或者泊位）路径

    
    Robot(int id, int startX, int startY) : robot_id(id), x(startX), y(startY) {
        status = 1;
        goods = 0;
    }

    // 是否空闲
    bool is_idle();

};