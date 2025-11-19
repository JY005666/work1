#include "pid.h"
void positionSeror(float cur,Speed_PID *speed_pid,Pos_PID *pos_pid){
    pos_pid->current = cur;
    Pos_PID_Calculate(pos_pid);

    speed_pid->current = pos_pid->error[1]-pos_pid->error[0];//将两次误差差值作为当前速度值
    Speed_PID_Calculate(speed_pid);
}