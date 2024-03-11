#pragma once

#include <vector>
#include <list>
#include <unordered_map>
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
    unordered_map<int, Goods> goods;

    vector<vector<list<int>>> path_to_berth = vector<vector<list<int>> >(200, vector<list<int>>(200)); // 每个点到最近的berth的路径
    vector<vector<int>> path_to_berth_id = vector<vector<int> >(200, vector<int>(200, -1));;            // 每个点的目标berth id，-1表示不能到达任意一个berth

    // 初始化每个点到最近berth的路径
    void init_path_to_berth();

    // 初始化机器人，设定其是否有效
    void init_robots();

    // 判断某个点能否到达berth
    bool valid_to_berth(int _x, int _y);
};