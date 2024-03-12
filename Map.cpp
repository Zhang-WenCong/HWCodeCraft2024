#include "Map.h"
#include "PathPlanning.h"

// 初始化每个点到最近berth的路径
void Map::init_path_to_berth() {
    for(auto& berth : berths) {
        bfs_get_full_path(berth.x, berth.y, berth.berth_id, this->char_map, this);
    }
}

// 初始化机器人，设定其是否有效
void Map::init_robots() {
    for(auto& robot : robots) {
        if(!valid_to_berth(robot.x, robot.y))
            robot.is_valid = false;
    }
}

// 判断某个点能否到达berth
bool Map::valid_to_berth(int _x, int _y) {
    return path_to_berth_id[_x][_y] != -1;
}