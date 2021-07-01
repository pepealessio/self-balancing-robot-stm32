/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "led.h"
#include "mpu6050_k.h"
#include "pid_controller.h"
#include "supervisor.h"
#include "hbridge.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Equilibrium PID parameters
#define KP_ANGLE 		(120)
#define KI_ANGLE		(80)
#define KD_ANGLE 		(90)

#define DT 				(15)

// Uncomment if you want use UART2 TX pin (same on USB, speed 115200) to read
// actual angle and cycle time.
//#define PRINT_DEBUG

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#if defined (PRINT_DEBUG)
// Buffer for debugging print
char buffer[10];
#endif

uint32_t tick;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Balancing_Error_Handler(void);
void print_on_UART(void);
void MPU_Error_Handler(void);

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  // Error led start
  Led__init(&hled0, GPIOA, GPIO_PIN_5);

  // Start motor controller
  hbridge__init(&hhbridge, &htim1);

  // Start MPU device
  if (MPU6050__Init(&hmpu, &hi2c1))
  {
	  MPU_Error_Handler();
  }

  // Start supervisor
  FSM__init(&hfsm);
  hfsm_buffer = 0x00;
  HAL_UART_Receive_IT(&huart1, &hfsm_buffer, 1);

  // Start pid
  pid__init(&hangle_pid, KP_ANGLE, KI_ANGLE, KD_ANGLE, FSM__ANGLE_OFFSET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	tick = HAL_GetTick();

	/* ---------------------------------- READ INPUT ---------------------------------- */
	// Read the tilt angle.
	if (MPU6050__Read_All(&hmpu))
	{
		MPU_Error_Handler();
	}

	// Check for SBR is stand up. If it fallen stop the motor and the interrupt and attemps
	// for a reboot.
	if (fabs(hmpu.KalmanAngleY) > 60)
	{
		Balancing_Error_Handler();
	}

	/* ---------------------------------- EVALUATE OUTPUT ---------------------------------- */
	// Evaluate offset and target using the control byte received from esp32.
	FSM__update(&hfsm);

	// Use pid to evaluate the actual needed motor power.
	int64_t tilt_pwm = pid__evaluate_output(&hangle_pid, (int)(hmpu.KalmanAngleY * 100));

	/* ---------------------------------- WRITE OUTPUT ---------------------------------- */
	// Now control the motor with the previous calculation.
	hbridge__set_motor(&hhbridge, tilt_pwm + hfsm.left_target, tilt_pwm + hfsm.right_target);

#if defined (PRINT_DEBUG)
	print_on_UART();
#endif

	HAL_Delay(DT - (HAL_GetTick() - tick));
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

void Balancing_Error_Handler(void)
{
	// Disable control message IRQ
	HAL_NVIC_DisableIRQ(USART1_IRQn);

	// Stop motor
	hbridge__set_motor(&hhbridge, 0, 0);

	// Blink led
	TIM10->PSC = 11999;
	TIM10->ARR = 6999;
	HAL_TIM_Base_Start_IT(&htim10);

	// Wait for reset
	while (1)
	{

	}
}

void MPU_Error_Handler(void)
{
	// Disable control message IRQ
	HAL_NVIC_DisableIRQ(USART1_IRQn);

	// Stop motor
	hbridge__set_motor(&hhbridge, 0, 0);

	// Blink led
	TIM10->PSC = 11999;
	TIM10->ARR = 3749;
	HAL_TIM_Base_Start_IT(&htim10);

	// Wait for reset
	while (1)
	{

	}
}

#if defined (PRINT_DEBUG)
void print_on_UART(void)
{
	HAL_UART_Transmit(&huart2, (uint8_t *) "x: ", 3, 0xFFFF);
	gcvt(hmpu.KalmanAngleY, 6, buffer);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);

	HAL_UART_Transmit(&huart2, (uint8_t *) "y: ", 3, 0xFFFF);
	gcvt(hmpu.KalmanAngleY, 6, buffer);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);

	HAL_UART_Transmit(&huart2, (uint8_t *) "el: ", 4, 0xFFFF);
	itoa((int) hencoderl.cnt, buffer, 10);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);

	HAL_UART_Transmit(&huart2, (uint8_t *) "er: ", 4, 0xFFFF);
	itoa((int) hencoderr.cnt, buffer, 10);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);

	HAL_UART_Transmit(&huart2, (uint8_t *) "c.b. : ", 5, 0xFFFF);
	itoa(hfsm.current_control_message, buffer, 10);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);

	HAL_UART_Transmit(&huart2, (uint8_t *) "el.t: ", 6, 0xFFFF);
	itoa(HAL_GetTick() - tick, buffer, 10);
	strcat(buffer, "\r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	HAL_UART_Transmit(&huart2, (uint8_t *) buffer, 10, 0xFFFF);
}
#endif

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
