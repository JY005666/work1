#include"speed_pid.h"

void Speed_PID_Init(float kp, float ki, float kd,float target,float current,float output_max,float integral_max,Speed_PID *pid){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0;
    pid->current = current;
    pid->error[0] = 0;
    pid->error[1] = 0;
    pid->output = 0;
    pid->integral_max = integral_max;
    pid->output_max = output_max;
}
void Speed_PID_Calculate(PID *pid){
    pid->error[0] = pid->target - pid->current;
    pid->integral += pid->error[0];
    if(pid->integral > pid->integral_max) pid->integral = pid->integral_max;
    else if(pid->integral < -pid->integral_max) pid->integral = -pid->integral_max;
    pid->output = pid->kp * pid->error[0] + pid->ki * pid->integral + pid->kd * (pid->error[0] - pid->error[1]);
    pid->error[1] = pid->error[0];
    if(pid->output > pid->output_max) pid->output = pid->output_max;
    else if(pid->output <0) pid->output = 0;
}