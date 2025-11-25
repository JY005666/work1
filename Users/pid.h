typedef struct{
    float kp;
    float ki;
    float kd;
    float error[2]; //将error[0]设置为此次误差，error[1]设置为上次误差
    float output; //输出值
    float integral;//积分值
    float target; //目标值
    float current; //当前值
    float integral_max; //积分值最大值
    float output_max;//输出值最大值
} PID;
void PID_Init(float kp, float ki, float kd, PID *pid);
void PID_Calculate(PID *pid);