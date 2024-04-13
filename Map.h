#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include "Robot.h"
#include "Berth.h"
#include "Boat.h"
#include "Goods.h"
#include "common.h"

using namespace std;

class Map {
public:
    int height = 200;
    int width = 200;
    int n_robots = 10, n_berths = 10, n_boats = 5;

    int type = 3;
    
    vector<vector<char>> char_map = vector<vector<char> >(200, vector<char>(200));
    vector<Robot> robots;
    vector<Berth> berths;
    vector<Boat>  boats;

    // 机器人购买地
    vector<pair<int, int>> robot_purchase_point;
    // 船只购买地
    vector<pair<int, int>> boat_purchase_point;
    // 交货点
    vector<pair<int, int>> delivery_point;
    
    unordered_map<int, Goods> goods;
    vector<priority_queue<Goods> > goods_queue = vector(10, priority_queue<Goods>());

    // [i][j][k] 表示坐标i，j到泊位k的下一步动作
    vector<vector<vector<int>>> path_to_berth = vector<vector<vector<int>> >(200, vector<vector<int>>(200, vector<int>(BERTH_N, -1)));
    vector<vector<vector<int>>> path_to_berth_len = vector<vector<vector<int>> >(200, vector<vector<int>>(200, vector<int>(BERTH_N, 100000)));
    vector<vector<int>> path_to_berth_id = vector<vector<int> >(200, vector<int>(200, -1));           // 每个点的目标berth id，-1表示不能到达任意一个berth

    // 初始化每个点到最近berth的路径
    void init_path_to_berth();

    // 判断某个点能否到达berth
    bool valid_to_berth(int _x, int _y);

    void show();
};