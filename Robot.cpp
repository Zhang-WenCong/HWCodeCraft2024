#include "Robot.h"

bool Robot::is_idle() {
    return status == 1 && goods == 0 && !in_task;
}