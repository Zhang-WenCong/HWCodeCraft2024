#include <iostream>
#include "Map.h"
#include "common.h"
#include "Solver.h"

using namespace std;

void Init(Map& map) {
    char line[1024];
    int robots_n = 0;
    int boat_capacity;

    // 读取地图数据
    for(int row = 0; row < MAP_H; row++) {
        scanf("%s", line);
        for (int col = 0; col < MAP_W; col++) {
            map.char_map[row][col] = line[col];

            if (line[col] == 'A') {
                Robot robot(robots_n, row, col);
                map.robots.push_back(robot);
                robots_n++;
                map.char_map[row][col] = '.';
            }
        }
    }
        
    for(int i = 0; i < BERTH_N; i++) {
        Berth berth;
        scanf("%d%d%d%d%d", &berth.berth_id, &berth.x, &berth.y, &berth.transport_time, &berth.loading_speed);
        map.berths.push_back(berth);
    }

    scanf("%d", &boat_capacity);
    for (int i = 0; i < BOAT_N; i++) {
        map.boats.push_back({i, boat_capacity});
    }
    
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int main() {
    Map map;
    Init(map);
    Solver solver(&map);

    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        solver.get_frame(frameID);

        solver.output_frame();

        puts("OK");
        fflush(stdout);
    }

    return 0;
}
