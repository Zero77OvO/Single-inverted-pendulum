/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "interrupt.h"
#include "adcc.h"
#include "string.h"
#include "stdio.h"
#include "pid.h"
#include "vofa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern char rxdatas[30];
extern uint8_t rxdata;
extern unsigned char rx_pointer;
extern int flag_stop;
int flag_init = 0;
extern int num_counter;

//extern uint8_t rx_buff;	

extern float v_out;
extern float x_out;

extern int start_mode ;

extern int alarm_left ;
extern int alarm_right ;
extern int raw_now; 
extern int raw_filter;
extern float err_x;
extern int target_angle; 
int timer_sign=0;
int c=0,b=0;
extern float t2;
extern float t;
extern float location;


 int flag_jc12=0;
 int flag_jc3=0;
int flag_fh=0;

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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start(&htim1);
	
	HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start(&htim3);
	
	HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start(&htim4);
	
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_ADC_Start(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_UART_Receive_IT(&huart3, &rxdata, 1);
  //HAL_UART_Receive_IT(&huart3, (uint8_t *)&rx_buff, 1);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

  __HAL_TIM_SET_COUNTER(&htim2, 30000);
	

 raw_now = (int)(get_adc(&hadc1) * 100);

 num_counter = __HAL_TIM_GET_COUNTER(&htim2);
 extern int target_location;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    OLED_ShowNum(0, 0, raw_filter, 10, 16, 1);
    OLED_ShowNum(0, 2,  location, 6, 16, 1); 
    OLED_ShowNum(0, 4, raw_filter-location, 6, 16, 1);  if(x_out<0)OLED_ShowNum(40, 4, -x_out, 6, 16, 1);  
		OLED_ShowNum(0, 6, start_mode, 6, 16, 1);
   // vodka_JustFloat_send(&huart3);
    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
     //À¶ÑÀÊµÏÖÌâÄ¿ÇÐ»»£¬
    ///////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ///

    if (rx_pointer >= 3)
    {
      if (rxdatas[0] == 0 && rxdatas[1] == 0) //ÃÜÂë00
      {
        if (rxdatas[2] == 0 ) 
        { 
					flag_stop=1;flag_jc12=0;flag_jc3=0;flag_fh=0;

        }
        if (rxdatas[2] == 1 ) /////////////////////ÓÒ
        {
         //»ù´¡12
					flag_jc12=1;flag_stop=0;
        }
        if (rxdatas[2] == 2)   
        {
          flag_jc3=1; flag_stop=0;
        }
        if (rxdatas[2] == 3)  
        {
        
					start_mode=-1;flag_stop=0;
					
        }
				
				
				
        rx_pointer = 0;
        memset(rxdatas, 0, 30);
      }
    }

    if (flag_stop == 1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    }
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
