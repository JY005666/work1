#include"stdint.h"
#include"pid.h"
void Motor_SetSpeed(float speed);
void motor_position_set(uint32_t position,PID *pid);
void motor_position_set_2(uint32_t position,PID *pid);
void motor_position_set_3(uint32_t position,PID *pid);
void motor_position_set_4(uint32_t position,PID *pid);