#pragma once

#include "common.h"

int convert_xy(int _x, int _y);

void predict_nxy(int _x, int _y, int move, int& nx, int& ny);

int reverse_dir(int dir);