#pragma once

#define DEBUG

const int MAP_H = 200;  // 地图h
const int MAP_W = 200;  // 地图w
const int ROBOT_N = 10; // 机器人数量
const int BERTH_N = 10; // 泊位数量
const int BOAT_N = 5;   // 船只数量

const int robot_price = 2000;
const int boat_price = 8000;

const int GOODS_LIFE = 1000;  // 物品存活时间

// 定义方向 右 左 上 下
const int dx[] = {0, 0, -1, 1};
const int dy[] = {1, -1, 0, 0};

