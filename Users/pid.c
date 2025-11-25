#include"pid.h"

void PID_Init(float kp, float ki, float kd, PID *pid){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->error[0] = 0;
    pid->error[1] = 0;
    pid->output = 0;
    pid->integral = 0;
    pid->current = 0;
    pid->integral_max = 1000.0f;
    pid->output_max = 1000.0f;
}
void PID_Calculate(PID *pid){
    pid->error[0] = pid->target - pid->current;
    pid->integral += pid->error[0];
    if(pid->integral > pid->integral_max) pid->integral = pid->integral_max;
    else if(pid->integral < -pid->integral_max) pid->integral = -pid->integral_max;
    pid->output = pid->kp * pid->error[0] + pid->ki * pid->integral + pid->kd * (pid->error[0] - pid->error[1]);
    pid->error[1] = pid->error[0];
    if(pid->output > pid->output_max) pid->output = pid->output_max;
    else if(pid->output < -pid->output_max) pid->output = -pid->output_max;
}