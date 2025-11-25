#include"stdint.h"
#include"pid.h"
void Motor_SetSpeed(float speed);
void motor_position_set(uint32_t position,PID *pid);