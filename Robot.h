#pragma once

class Robot {
public:
    int robot_id;
    int x, y, goods;
    int status;    
    int mbx, mby;

    
    Robot(int id, int startX, int startY) : robot_id(id), x(startX), y(startY) {
        status = 1;
        goods = 0;
    }

};