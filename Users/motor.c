#include "tim.h"
#include "gpio.h"
#include"motor.h"
void Motor_SetSpeed(float speed){
    if(speed>=0){//正转
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin,1 );
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);//通过调节cnt改变占空比进而控制速度
    }
    else{//反转
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin,0);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin,1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, -speed);
    }
}
 