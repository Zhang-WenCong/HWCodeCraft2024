#pragma once

#include <vector>
#include "Robot.h"
#include "Berth.h"
#include "Boat.h"
#include "Goods.h"

using namespace std;

class Map {
public:
    int height = 200;
    int width = 200;
    int n_robots = 10, n_berths = 10, n_boats = 5;
    
    vector<vector<char>> char_map = vector<vector<char> >(200, vector<char>(200));
    vector<Robot> robots;
    vector<Berth> berths;
    vector<Boat>  boats;
    vector<Goods> goods;
};