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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "key.h"
#include "servo.h"
#include "arm_math.h"
#include "chess.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

#define FRAME_W   800
#define FRAME_H   480
//#define FRAME_CX  400
//#define FRAME_CY  240

// ���ջ��塢��־
uint8_t  rx_buf[6];
volatile uint8_t rx_flag = 0;



int16_t count_1ms, count_10ms,count_1s,count_25ms,count_200ms;

uint8_t test_start_flag=1;
uint8_t test_num=1;
uint8_t key_num=0,receive_flag=0,test1_flag=1,PID_START_FALG=0,SERVO_START_FALG = 0,test2_flag=1,test2_start_flag;
uint8_t test_show;


extern uint8_t r_data,c_data,chess_set_error,row_error_chess,col_error_chess,row_error_null,col_error_null;


uint8_t test4_AIMOVE,TIME_DATA,test4_move,test4_init,test4_chess=1,test4_win_flag;

uint8_t test5_AIMOVE,test5_move,test5_init,test5_chess=6,test5_win_flag,test5_errorfind,test5_start;


uint8_t LED_FLAG;

uint8_t RX_data[10],data[10];

uint8_t time_flag;

char board[3][3];

//uint32_t DATA_coordinate[70],GOAL_data[2],chessbroad_data[36];

uint8_t tast2_buff[4];

/************��ͷСͷת������****************/
uint32_t swap_uint32(uint32_t value) 
{
	    return ((value << 24) & 0xFF000000) |
           ((value << 8)  & 0x00FF0000) |
           ((value >> 8)  & 0x0000FF00) |
           ((value >> 24) & 0x000000FF);
}


/************���ڻص�����****************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        if (rx_buf[0] == 0xE8)
        {
            rx_flag = 1;
        }
        // ����������һ�ν���
        HAL_UART_Receive_IT(&huart2, rx_buf, 6);
    }
}
/************��ʱ���ص�����****************/
void 	HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {
		
			/* ��ʱ��1��ʱ����*/
			if(htim ->Instance == TIM1 )
			{

					count_1ms++;
					count_10ms++;
				  count_1s++;
				  count_25ms++;
					count_200ms++;
//					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
					if(count_1ms == 1)
					{	
						

          
						count_1ms=0;
					}
					if(count_10ms == 10)
					{		
						count_10ms=0;
					}
					if(count_25ms == 25)
					{		
						key_num = key_loop();	
						if(test4_AIMOVE == 1)
						{
							if(key_num ==8)
							{
								HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
								LED_FLAG =0;
								test4_move = 1;
								test4_AIMOVE =0;
								
							}
						}
						if(test5_AIMOVE == 1)
						{
							if(key_num ==8)
							{
								HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
								LED_FLAG =0;
								test5_move = 1;
								test5_AIMOVE =0;
								
							}
						}
						if(key_num ==12)
						{
							test5_start =1;
						}

						count_25ms=0;
					}
					
					if(count_200ms==20)
					{
//							OLED_Clear();
						HAL_UART_Receive_IT(&huart2, rx_buf, 6);
						
						
						count_200ms=0;
					}
					
					if(count_1s==1000)
					{
						
						/*����ָʾ��*/
						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);
						time_flag++;
						count_1s=0;
//						HAL_UART_Transmit(&huart1,TX_data,sizeof(TX_data),1000); 
//		        HAL_UART_Transmit(&huart2,TX_data,sizeof(TX_data),1000); 
					}
			}

  }

	
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	OLED_Init(); 
	OLED_Clear(); 
	pwm_start(); 
	servo_reset();
	HAL_UART_Receive_IT(&huart2, rx_buf, 6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
    {
        if (rx_flag)
        {
            // 解析高低字节：大端模式
            uint16_t px =  (uint16_t)rx_buf[1]
             | ((uint16_t)rx_buf[2] << 8);
					
						uint16_t py =  (uint16_t)rx_buf[3]
             | ((uint16_t)rx_buf[4] << 8);

            //servo_pid_h(px, FRAME_CX);
						//servo_pid_w(py, FRAME_CY);
						servo_pid(px, FRAME_CX, py, FRAME_CY);

            rx_flag = 0;
        }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
