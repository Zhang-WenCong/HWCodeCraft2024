#pragma once


class Goods {
public:
    int x, y;
    int value;
    int gen_frame; // 生成时的帧数，过1000帧不减会消失

    Goods(int x, int y, int v, int f) : x(x), y(y), value(v), gen_frame(f) {}

};