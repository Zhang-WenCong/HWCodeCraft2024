#include "Berth.h"

bool Berth::is_in_berth(int _x, int _y) {
    return (_x <= x + 3) && (_y <= y + 3) && (_x >= x) && (_y >= y);
}