#pragma once

#include "Map.h"
#include "common.h"




class Solver {
public:
    Map* map;

    int cur_frame, money;

    static int goods_id;

    Solver(Map* _map) : map(_map) {
    }

    void get_frame(int frame_id_input);

    int get_idle_robots_num();

    int get_idle_goods_num();

    int remove_expired_goods();

    void update_status();

    void update_assign_tasks();

    void output_frame();

};