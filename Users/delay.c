#include "delay.h"
#include"tim.h"
void delay(uint16_t us)
{
    uint16_t counter=0;
    __HAL_TIM_SetCounter(&htim3,counter);
    HAL_TIM_Base_Start(&htim3);

    while(counter<us)
    {
        counter=__HAL_TIM_GetCounter(&htim3);
    }
    HAL_TIM_Base_Stop(&htim3);
}