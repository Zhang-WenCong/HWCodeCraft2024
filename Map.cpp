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

    // fprintf(stderr, "\n");

    for(auto& berth : berths) {
        berth.show();
    }
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
                path_to_berth_id[berth.x + i][berth.y + j] = berth.berth_id;
            }
        }
    }
}

// 判断某个点能否到达berth
bool Map::valid_to_berth(int _x, int _y) {
    return _x >= 0 && _x < 200 && _y >= 0 && _y < 200 && path_to_berth_id[_x][_y] != -1;
}