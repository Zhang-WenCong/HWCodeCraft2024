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
        bfs_get_full_canto(berth.x, berth.y, berth.berth_id, this->char_map, this);
    }

    for (auto& berth : berths) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                path_to_berth[berth.x + i][berth.y + j][berth.berth_id] = -1;
            }
        }
    }

}

// 初始化机器人，设定其是否有效和初始泊位
void Map::init_robots() {
    bool used_berth[BERTH_N] = {false};
    for(auto& robot : robots) {
        if(!valid_to_berth(robot.x, robot.y))
            robot.is_valid = false;
        else {
            if(!used_berth[path_to_berth_id[robot.x][robot.y]]) {
                robot.target_berth_id = path_to_berth_id[robot.x][robot.y];
                used_berth[path_to_berth_id[robot.x][robot.y]] = true;
            }
        }
    }
    for(auto& robot : robots) {
        if(!robot.is_valid || robot.target_berth_id != -1)
            continue;
        else {
            for(int i = 0; i < BERTH_N; i++) {
                if(!used_berth[i] && path_to_berth[robot.x][robot.y][i] != -1){
                    robot.target_berth_id = i;
                    used_berth[path_to_berth_id[robot.x][robot.y]] = true;  
                    break;
                }
            }
        }
    }
    for(auto& robot : robots) {
        if(!robot.is_valid || robot.target_berth_id != -1)
            continue;
        else {
            for(int i = 0; i < BERTH_N; i++) {
                if(path_to_berth[robot.x][robot.y][(i + robot.robot_id) % BERTH_N] != -1){
                    robot.target_berth_id = (i + robot.robot_id) % BERTH_N;
                    break;
                }
            }
        }
    }
}

// 判断某个点能否到达berth
bool Map::valid_to_berth(int _x, int _y) {
    return path_to_berth_id[_x][_y] != -1;
}