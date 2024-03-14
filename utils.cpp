#include "utils.h"

int convert_xy(int _x, int _y) {
    return MAP_W * _x + _y;
}

void predict_nxy(int _x, int _y, int move, int& nx, int& ny) {
    nx = _x + dx[move];
    ny = _y + dy[move];
}

int reverse_dir(int dir) {
    if(dir == 0)
        return 1;
    else if(dir == 1)
        return 0;
    else if(dir == 2)
        return 3;
    else if(dir == 3)
        return 2;
    return -1;
}