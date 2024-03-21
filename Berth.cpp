#include "Berth.h"
#include <math.h>
#include <iostream>

bool Berth::is_in_berth(int _x, int _y) {
    return (_x <= x + 3) && (_y <= y + 3) && (_x >= x) && (_y >= y);
}


int Berth::get_abs_len(int _x, int _y) {
    return abs(x - _x) + abs(y - _y);
}

void Berth::show() {
    fprintf(stderr, "Berth %d (x:%d, y:%d, transport_time:%d loading_speed:%d)\n", berth_id, x, y, transport_time, loading_speed);
}