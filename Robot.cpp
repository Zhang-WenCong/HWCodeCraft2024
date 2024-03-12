#include "Robot.h"
#include <iostream>

bool Robot::is_idle() {
    return status == 1 && goods == 0 && task_type == -1 && is_valid;
}

void Robot::show() {
    fprintf(stderr, "Robot %d (%d, %d, %d %d), is_valid %d\n", robot_id, x, y, goods, status, is_valid);
    fprintf(stderr, "task_type %d, tar_goods_id (%d)\n", task_type, target_goods_id);
    if(!path_to_goods.empty())
        fprintf(stderr, "next move : %d \n", path_to_goods.front());
}