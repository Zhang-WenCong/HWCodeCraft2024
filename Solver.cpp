#include "Solver.h"
#include "common.h"
#include "PathPlanning.h"
#include <iostream>

int convert_xy(int _x, int _y) {
    return MAP_W * _x + _y;
}

int Solver::goods_id = 0;

void Solver::get_frame(int frame_id_input) {
    this->cur_frame = frame_id_input;
    scanf("%d", &(this->money));

    int num;                    
    scanf("%d", &num);          // num:新增货物数量
    for(int i = 0; i < num; i ++) {
        int x, y, val;          // 货物坐标和金额
        scanf("%d%d%d", &x, &y, &val);
        map->goods[goods_id++] = Goods(x, y, val, cur_frame);
    }

    for(int i = 0; i < ROBOT_N; i ++)
        scanf("%d%d%d%d", &map->robots[i].goods, &map->robots[i].x, &map->robots[i].y, &map->robots[i].status);

    for(int i = 0; i < BOAT_N; i ++)
        scanf("%d%d\n", &map->boats[i].status, &map->boats[i].target_berth);
    
    char okk[100];
    scanf("%s", okk);

    
    // 如果有分配的货物消失，或者有空闲货物同时有空闲robot，更新任务分配
    int remove_num = remove_expired_goods();
    if(remove_num > 0 || (get_idle_goods_num() > 0 && get_idle_robots_num() > 0)) {
        update_assign_tasks();
    }
}

int Solver::get_idle_robots_num() {
    int count = 0;
    for(auto& robot : map->robots)
        if(robot.is_idle())
            count++;
    return count;
}

int Solver::get_idle_goods_num() {
    int count = 0;
    for(auto it = map->goods.begin(); it != map->goods.end(); ++it) {
        if(it->second.assigned_robot_id == -1) {
            count++;
        }
    }
    return count;
}

int Solver::remove_expired_goods() {
    int remove_num = 0;
    for(auto it = map->goods.begin(); it != map->goods.end(); /* 不在这里递增 */) {
        if (cur_frame >  it->second.gen_frame + GOODS_LIFE) { // 如果过期了
            if(it->second.assigned_robot_id != -1) { // 如果被分配了但是没被捡走，就要让分配的机器人重新选一个货物
                remove_num++;
                map->robots[it->second.assigned_robot_id].in_task = false;
            }
            it = map->goods.erase(it); // erase 后 it 自动更新为下一个元素的迭代器
        } else {
            ++it; // 手动递增
        }
    }
    return remove_num;
}

void Solver::update_assign_tasks() {
    // 先随机分配，之后换成匈牙利匹配算法
    for(auto& robot : map->robots) {
        if(robot.is_idle()) {
            for(auto it = map->goods.begin(); it != map->goods.end(); ++it) {
                if(it->second.assigned_robot_id == -1) {
                    // 用a*算法查看是否有路径，有就分配
                    if (astar_get_two_path(robot.x, robot.y, it->second.x, it->second.y, map->char_map,
                                            robot.path, map)) {
                        robot.in_task = true;
                        robot.target_x = it->second.x;
                        robot.target_y = it->second.y;
                        it->second.assigned_robot_id = robot.robot_id;
                        break;
                    }
                }
            }
        }
    }
}

void Solver::output_frame() {
    for(auto& robot : map->robots) {
        if(robot.status == 0) {
            robot.path.front().second = 0;
            continue;
        }
        if(robot.is_idle()) {
            // 未来可能让其向远离其他机器人方向移动
            continue;
        }
        // path非空，就根据path移动
        if(!robot.path.empty()) {
            printf("move %d %d\n", robot.robot_id, robot.path.front().first);
            robot.path.pop_front();
        }
        // 到达目的地
        if(robot.path.empty()) {
            if(robot.goods == 0)
                printf("get %d\n", robot.robot_id);
            else {
                printf("pull %d\n", robot.robot_id);
            } 
        }
    }
}