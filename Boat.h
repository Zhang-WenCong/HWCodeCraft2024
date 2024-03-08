#pragma once


class Boat {
public:
    int boat_id;
    int capacity;
    int status;         // 0 移动中； 1 装货状态或运输完成状态； 2 表示泊位外等待状态 
    int target_berth;   // 目标泊位，-1就是虚拟节点

    Boat(int id, int capacity) : boat_id(id), capacity(capacity) {
        status = 0;
        target_berth = -1;
    }
};