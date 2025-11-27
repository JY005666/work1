/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"motor.h"
#include"stdio.h"//11
#include"stdlib.h"
#include"stepper.h"
// test2
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t time_flag = 0;  //定时器中断的判断标志
uint8_t current_mode = 1;  //小车电机的运动状态
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  stepper_init();
  PID pid;
  PID_Init(0.5,0.5,1.0,&pid); // 初始化pid
  HAL_TIM_Base_Start_IT(&htim1); 
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  //启动PWM
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);//启动编码器
  HAL_Delay(2000);
  stepper_step(100,1,300);
  __HAL_TIM_SET_COUNTER(&htim2,30000); //设置编码器计数值,防止开始时反向移动轮子出现问题
  while(current_mode==1){ //移动到砝码后
      motor_position_set(2*11*26*2*2-100, &pid,2);
  }
    __HAL_TIM_SET_COUNTER(&htim2,30000); 
      stepper_step(280,0,500);
    while(current_mode==2)//到达能够勾起砝码的位置
    {
      motor_position_set(-200, &pid,3);
    }
      __HAL_TIM_SET_COUNTER(&htim2,30000); 
      stepper_step(800,1,300);//上钩
      while(current_mode==3)
      {
        motor_position_set(-2*11*26*2*2-150,&pid,4);//到达砝码放下的位置
      }
      __HAL_TIM_SET_COUNTER(&htim2,30000); 
      stepper_step(250,0,3000);
      while(current_mode==4){
        motor_position_set(2*11*25*2*2, &pid,0);//回到起始线左边
      }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1      ) //10ms中断
    {
        time_flag=1; //设置标志位
    }
}
void motor_position_set(int32_t position,PID *pid,uint8_t next_mode)//正转
{
    int32_t cnt;
    pid->target = position+30000; //设置目标值
    if(time_flag){
        cnt=__HAL_TIM_GET_COUNTER(&htim2); //读取编码器计数值
        pid->current = cnt; //更新当前值...
        if(labs(pid->current-pid->target)>15){  //当实际值与目标值偏差大于一定值时，才进行PID计算和电机控制
          PID_Calculate(pid); //计算PID
          Motor_SetSpeed((pid->output+100)/2); //设置电机速度
          printf("%.3f,%.3f,%.3f\r\n",pid->target,pid->current,pid->output);
          time_flag=0; //清除标志位
        }
        else{//到达目标位置附近，停止电机
          pid->output=0;
          Motor_SetSpeed(0);//停止电机
          time_flag=0; //清除标志位
         current_mode=next_mode;
          // HAL_Delay(2000);
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
