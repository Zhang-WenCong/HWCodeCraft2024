#pragma once


class Goods {
public:
    int x, y;
    int value;
    int gen_frame; // 生成时的帧数，过1000帧会消失

    int assigned_robot_id = -1; // 分配的robot id，-1表示未分配

    Goods(int x, int y, int v, int f) : x(x), y(y), value(v), gen_frame(f) {}

};