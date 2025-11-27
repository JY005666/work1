#include"stdint.h"
#include"pid.h"
void Motor_SetSpeed(float speed);
void motor_position_set(int32_t position,PID *pid,uint8_t next_mode);