#include <iostream>
#include "Map.h"
#include "common.h"

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

int id, money;

void Input(Map& map) {
    scanf("%d%d", &id, &money); // id：帧序号  money：当前金钱数

    int num;                    
    scanf("%d", &num);          // num:新增货物数量
    for(int i = 1; i <= num; i ++) {
        int x, y, val;          // 货物坐标和金额
        scanf("%d%d%d", &x, &y, &val);
        map.goods.push_back({x, y, val, id});
    }

    for(int i = 0; i < ROBOT_N; i ++)
        scanf("%d%d%d%d", &map.robots[i].goods, &map.robots[i].x, &map.robots[i].y, &map.robots[i].status);

    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &map.boats[i].status, &map.boats[i].target_berth);
    
    char okk[100];
    scanf("%s", okk);
}

int main() {
    Map map;
    Init(map);
    for(int frame = 1; frame <= 15000; frame++) {
        Input(map);
        for(int i = 0; i < ROBOT_N; i++)
            printf("move %d %d\n", i, rand() % 4);
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
