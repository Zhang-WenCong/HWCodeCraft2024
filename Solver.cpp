#include "Solver.h"
#include "common.h"
#include "PathPlanning.h"
#include "utils.h"
#include <iostream>
#include <unordered_set>


int Solver::goods_id = 0;

void Solver::get_frame(int frame_id_input) {
    this->cur_frame = frame_id_input;
    scanf("%d", &(this->money));

    int num;                    
    scanf("%d", &num);          // num:新增货物数量
    for(int i = 0; i < num; i ++) {
        int x, y, val;          // 货物坐标和金额
        scanf("%d%d%d", &x, &y, &val);
        if(map->valid_to_berth(x, y)) {
            map->goods[goods_id] = Goods(x, y, val, cur_frame, goods_id);
            goods_id++;
        }
    }

    for(int i = 0; i < ROBOT_N; i ++) {
        scanf("%d%d%d%d", &map->robots[i].goods, &map->robots[i].x, &map->robots[i].y, &map->robots[i].status);
    }
    for(int i = 0; i < BOAT_N; i ++)
        scanf("%d%d\n", &map->boats[i].status, &map->boats[i].target_berth);
    
    char okk[100];
    scanf("%s", okk);
}

void Solver::update_status() {
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
    for(auto it = map->goods.begin(); it != map->goods.end(); ++it)
        if(it->second.assigned_robot_id == -1)
            count++;
    return count;
}

int Solver::remove_expired_goods() {
    int remove_num = 0;
    for(auto it = map->goods.begin(); it != map->goods.end(); /* 不在这里递增 */) {
        if (cur_frame >  it->second.gen_frame + GOODS_LIFE) { // 如果过期了
            // 如果被分配了但是没被捡走，就要让分配的机器人重新选一个货物
            if(it->second.assigned_robot_id != -1 && map->robots[it->second.assigned_robot_id].goods == 0 
                    && map->robots[it->second.assigned_robot_id].target_goods_id == it->second.goods_id) { 
                remove_num++;
                map->robots[it->second.assigned_robot_id].task_type = -1;
                map->robots[it->second.assigned_robot_id].target_goods_id = -1;
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
#ifdef DEBUG
        robot.show();
#endif
        if(!robot.is_idle())
            continue;
        for(auto it = map->goods.begin(); it != map->goods.end(); ++it) {
            if(it->second.assigned_robot_id != -1 || map->path_to_berth_id[it->second.x][it->second.y] != robot.target_berth_id) 
                continue;
            // 用a*算法查看是否有路径，有就分配, 同时分配货物到泊位路径图（已经预计算过）
            if (astar_get_two_path(robot.x, robot.y, it->second.x, it->second.y, 
                                    map->char_map, robot.path_to_goods, map)) {
                robot.task_type = 0;
                robot.target_goods_id = it->second.goods_id;
                it->second.assigned_robot_id = robot.robot_id;
                break;
            }
        }
        // 没分到
        if(robot.is_idle()) {
            for(auto it = map->goods.begin(); it != map->goods.end(); ++it) {
                if(it->second.assigned_robot_id != -1 || 
                        map->path_to_berth[it->second.x][it->second.y][robot.target_berth_id] == -1) 
                    continue;
                // 用a*算法查看是否有路径，有就分配, 同时分配货物到泊位路径图（已经预计算过）
                if (astar_get_two_path(robot.x, robot.y, it->second.x, it->second.y, 
                                        map->char_map, robot.path_to_goods, map)) {
                    robot.task_type = 0;
                    robot.target_goods_id = it->second.goods_id;
                    it->second.assigned_robot_id = robot.robot_id;
                }
                break;
            }
        }
    }
}

void Solver::output_frame() {
    unordered_set<int> nxy_set; // 记录当前所有机器人下一步路径，如果已经有机器人要走这个路径，就暂停避免碰撞
    unordered_set<int> cur_xy_set;  // 记录当前所有机器人的位置，用于检测对撞
    // 机器人动作
    for(auto& robot : map->robots) {
#ifdef DEBUG
            robot.show();
#endif
        cur_xy_set.insert(convert_xy(robot.x, robot.y));
        if(!robot.is_valid)
            continue;
        if(robot.status == 0) {
            nxy_set.insert(convert_xy(robot.x, robot.y));
            continue;
        }
        if(robot.is_idle()) {
            // 未来可能让其向远离其他机器人方向移动
            nxy_set.insert(convert_xy(robot.x, robot.y));
            continue;
        }
        int nx = robot.x, ny = robot.y;
        switch (robot.task_type) {
        case -1: // 可能送货最后一步碰撞导致送货失败
            if(robot.goods == 1) {
                robot.task_type = 1;
                nxy_set.insert(convert_xy(robot.x, robot.y));
            }
            break;
        case 0: // 取货
            // path非空，就根据path移动
            if(!robot.path_to_goods.empty()) {
                predict_nxy(robot.x, robot.y, robot.path_to_goods.front(), nx, ny);
                // 下一步不会撞到其他机器人，同时不会撞到障碍，也没有对撞
                if(nxy_set.find(convert_xy(nx, ny)) == nxy_set.end() && map->valid_to_berth(nx, ny) &&
                    !(cur_xy_set.find(convert_xy(nx, ny)) != cur_xy_set.end() && nxy_set.find(convert_xy(robot.x, robot.y)) != nxy_set.end())) {
                    printf("move %d %d\n", robot.robot_id, robot.path_to_goods.front());
                    nxy_set.insert(convert_xy(nx, ny));
                    robot.path_to_goods.pop_front();
                }else if (!map->valid_to_berth(nx, ny)) {
                    // 如果要撞到障碍了，说明可能发生跳帧，这一步不走，重新计算路径
                    astar_get_two_path(robot.x, robot.y, map->goods[robot.target_goods_id].x, map->goods[robot.target_goods_id].y, 
                                    map->char_map, robot.path_to_goods, map);
                    nxy_set.insert(convert_xy(robot.x, robot.y));
                }else if(cur_xy_set.find(convert_xy(nx, ny)) != cur_xy_set.end() && nxy_set.find(convert_xy(robot.x, robot.y)) != nxy_set.end()) {
                    // 如果对撞，就往没障碍的方向让一步
                    for(int i = 0; i < 4; i++) {
                        predict_nxy(robot.x, robot.y, i, nx, ny);
                        if(map->valid_to_berth(nx, ny) && nxy_set.find(convert_xy(nx, ny)) == nxy_set.end() 
                            && cur_xy_set.find(convert_xy(nx, ny)) == cur_xy_set.end()) {
                            printf("move %d %d\n", robot.robot_id, i);
                            robot.path_to_goods.push_front(reverse_dir(i));
                            nxy_set.insert(convert_xy(nx, ny));
                            break;
                        }
                    }
                }else {
                    nxy_set.insert(convert_xy(robot.x, robot.y));
                }
            }
            // 到达目的地
            if(robot.path_to_goods.empty()) {
                if(robot.goods == 0 && nx == map->goods[robot.target_goods_id].x && ny == map->goods[robot.target_goods_id].y) {
                    printf("get %d\n", robot.robot_id);
                    robot.task_type = 1;
                }else { // 某些原因如碰撞或者跳帧导致没有到达目标。重新计算路径
                    astar_get_two_path(robot.x, robot.y, map->goods[robot.target_goods_id].x, map->goods[robot.target_goods_id].y, 
                                    map->char_map, robot.path_to_goods, map);
                }
            }
            break;
        case 1: // 送货
            // 没有货物，可能是之前前往货物的路上最后一步碰撞了
            if(robot.goods == 0) {
                robot.task_type = 0;
                nxy_set.insert(convert_xy(robot.x, robot.y));
                break;
            }
            if(map->path_to_berth[robot.x][robot.y][robot.target_berth_id] != -1) {
                predict_nxy(robot.x, robot.y, map->path_to_berth[robot.x][robot.y][robot.target_berth_id], nx, ny);
                // 下一步不会撞到其他机器人，走
                if(nxy_set.find(convert_xy(nx, ny)) == nxy_set.end() && 
                     !(cur_xy_set.find(convert_xy(nx, ny)) != cur_xy_set.end() && nxy_set.find(convert_xy(robot.x, robot.y)) != nxy_set.end())) {
                    printf("move %d %d\n", robot.robot_id, map->path_to_berth[robot.x][robot.y][robot.target_berth_id]);
                    nxy_set.insert(convert_xy(nx, ny));
                }else if(cur_xy_set.find(convert_xy(nx, ny)) != cur_xy_set.end() && 
                        nxy_set.find(convert_xy(robot.x, robot.y)) != nxy_set.end()) {
                    // 如果对撞，就往没障碍的方向让一步
                    for(int i = 0; i < 4; i++) {
                        predict_nxy(robot.x, robot.y, i, nx, ny);
                        if(map->valid_to_berth(nx, ny) && nxy_set.find(convert_xy(nx, ny)) == nxy_set.end() 
                            && cur_xy_set.find(convert_xy(nx, ny)) == cur_xy_set.end()) {
                            printf("move %d %d\n", robot.robot_id, i);
                            robot.path_to_goods.push_front(reverse_dir(i));
                            nxy_set.insert(convert_xy(nx, ny));
                            break;
                        }
                    }
                }else {
                    nxy_set.insert(convert_xy(robot.x, robot.y));
                }
            }
            // 到达目的地
            if( map->berths[robot.target_berth_id].is_in_berth(nx, ny) && robot.goods == 1) {
                printf("pull %d\n", robot.robot_id);
                robot.task_type = -1;
                // 当前港口货物数量+1
                map->berths[robot.target_berth_id].cur_goods_num++;
            }
            break;
        default:
            break;
        }
    }

    // 船只动作
    for(auto& boat : map->boats) {
        switch (boat.status) {
        case 0: // 移动中
            break;
        case 1: // 装货状态或运输完成状态
            if(boat.target_berth == -1) { 
                printf("ship %d %d\n", boat.boat_id, boat.boat_id * 2 + rand() % 2);
                // if(map->berths[boat.boat_id * 2].cur_goods_num > map->berths[boat.boat_id * 2 + 1].cur_goods_num)
                //     printf("ship %d %d\n", boat.boat_id, boat.boat_id * 2);
                // else
                //     printf("ship %d %d\n", boat.boat_id, boat.boat_id * 2 + 1);
                boat.cur_goods = 0;
            }else {
                // 最后关头，赶紧走
                if(cur_frame + map->berths[boat.target_berth].transport_time >= 15000)
                    printf("go %d\n",boat.boat_id);
                else if(rand() % 300 == 1)
                    printf("go %d\n",boat.boat_id);
                // else if(boat.cur_goods >= boat.capacity / 2)
                //     printf("go %d\n",boat.boat_id);
                // else {
                //     int l_num = min(map->berths[boat.target_berth].cur_goods_num, map->berths[boat.target_berth].loading_speed);
                //     boat.cur_goods += l_num;
                //     map->berths[boat.target_berth].cur_goods_num -= l_num;
                // }

            }
            break;
        case 2: // 泊位外等待状态 
            break;
        default:
            break;
        }
    }
}