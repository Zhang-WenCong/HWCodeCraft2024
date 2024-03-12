#include "Map.h"
#include "PathPlanning.h"

void Map::show() {
    fprintf(stderr, "----- M A P S H O W -----\n");
    for (auto &robot : robots)  {
        robot.show();
    }
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            fprintf(stderr, "%c", char_map[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "----- M A P E N D -----\n");


    fprintf(stderr, "----- PATH SHOW -----\n");
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            if(path_to_berth_id[i][j] != -1)
                fprintf(stderr, "%d", path_to_berth_id[i][j]);
            else
                fprintf(stderr, ".");
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "----- PATH END -----\n");

    // auto tmp = path_to_berth[55][89];
    // while(!tmp.empty()) {
    //     fprintf(stderr, "->%d", tmp.front());
    //     tmp.pop_front();
    // }

    // fprintf(stderr, "\n");
}

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