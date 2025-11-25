#include "stepper.h"
#include "gpio.h"
#include"delay.h"
void stepper_init(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}
void stepper_step(uint32_t steps, uint8_t direction, uint16_t speed)
{
if(direction==0){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}
if(direction==1){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}
   for(int i=0;i<steps;i++)
   {
       
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
           HAL_Delay(speed);
           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
           HAL_Delay(speed);
    
   }    
}