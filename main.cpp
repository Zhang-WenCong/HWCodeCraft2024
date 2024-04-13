#include <iostream>
#include <ctime>
#include "Map.h"
#include "common.h"
#include "Solver.h"
#include "PathPlanning.h"

using namespace std;

void Init(Map& map) {
    char line[1024];
    int boat_capacity;

    // 读取地图数据
    for(int row = 0; row < MAP_H; row++) {
        scanf("%s", line);
        for (int col = 0; col < MAP_W; col++) {
            map.char_map[row][col] = line[col];

            if(line[col] == 'R') {
                map.robot_purchase_point.push_back(make_pair(row, col));
                map.char_map[row][col] = '>';
            }
            else if(line[col] == 'S') {
                map.boat_purchase_point.push_back(make_pair(row, col));
                map.char_map[row][col] = '~';
            }  
            else if(line[col] == 'T')
                map.delivery_point.push_back(make_pair(row, col));
        }
    }
    
    int berth_num;
    scanf("%d", &berth_num);
    for(int i = 0; i < berth_num; i ++)
    {
        int id, x, y, load_speed;
        scanf("%d%d%d%d", &id, &x, &y, &load_speed);
        // fprintf(stderr, "%d %d %d %d\n", id, x, y, load_speed);
        map.berths.push_back({id, x, y, load_speed});
    }
    
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    
    map.init_path_to_berth();

#ifdef DEBUG
    map.show();
#endif

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
        solver.update_status();
        solver.output_frame();
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
