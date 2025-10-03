/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c  LAB6
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usb_host.h"
#include "seg7.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
int DelayValue = 50;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
//void Play_Note(int note,int size,int tempo,int space);
//extern void Seven_Segment_Digit (unsigned char digit, unsigned char hex_char, unsigned char dot);
//extern void Seven_Segment(unsigned int HexValue);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char ramp = 0;
char RED_BRT = 0;
char GREEN_BRT = 0;
char BLUE_BRT = 0;
char RED_STEP = 1;
char GREEN_STEP = 2;
char BLUE_STEP = 3;
char DIM_Enable = 0;
char Music_ON = 0;
int TONE = 0;
int COUNT = 0;
int INDEX = 0;
int Note = 0;
int Save_Note = 0;
int Vibrato_Depth = 1;
int Vibrato_Rate = 40;
int Vibrato_Count = 0;
char Animate_On = 0;
char Message_Length = 0;
char *Message_Pointer;
char *Save_Pointer;
int Delay_msec = 0;
int Delay_counter = 0;

/* HELLO ECE-330L */
char Message[] =
		{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
		CHAR_H,CHAR_E,CHAR_L,CHAR_L,CHAR_O,SPACE,CHAR_E,CHAR_C,CHAR_E,DASH,CHAR_3,CHAR_3,CHAR_0,CHAR_L,
		SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE};

/* Declare array for Song */
Music Song[100];


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
  //MX_I2C1_Init();
  //MX_I2S3_Init();
  //MX_SPI1_Init();
  //MX_USB_HOST_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  /*** Configure GPIOs ***/
  GPIOD->MODER = 0x55555555; // set all Port D pins to outputs
  GPIOA->MODER |= 0x000000FF; // Port A mode register - make A0 to A3 analog pins
  GPIOE->MODER |= 0x55555555; // Port E mode register - make E8 to E15 outputs
  GPIOC->MODER |= 0x0; // Port C mode register - all inputs
  GPIOE->ODR = 0xFFFF; // Set all Port E pins high

  /*** Configure ADC1 ***/
  RCC->APB2ENR |= 1<<8;  // Turn on ADC1 clock by forcing bit 8 to 1 while keeping other bits unchanged
  ADC1->SMPR2 |= 1; // 15 clock cycles per sample
  ADC1->CR2 |= 1;        // Turn on ADC1 by forcing bit 0 to 1 while keeping other bits unchanged

  /*****************************************************************************************************
  These commands are handled as part of the MX_TIM7_Init() function and don't need to be enabled
  RCC->AHB1ENR |= 1<<5; // Enable clock for timer 7
  __enable_irq(); // Enable interrupts
  NVIC_EnableIRQ(TIM7_IRQn); // Enable Timer 7 Interrupt in the NVIC controller
  *******************************************************************************************************/

  TIM7->PSC = 199; //250Khz timer clock prescaler value, 250Khz = 50Mhz / 200
  TIM7->ARR = 1; // Count to 1 then generate interrupt (divide by 2), 125Khz interrupt rate to increment byte counter for 78Hz PWM
  TIM7->DIER |= 1; // Enable timer 7 interrupt
  TIM7->CR1 |= 1; // Enable timer counting

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* Jeopardy Song */
  Song[0].note = C4;
  Song[0].size = quarter;
  Song[0].tempo = 3000;
  Song[0].space = 10;
  Song[0].end = 0;

  Song[1].note = G4;
  Song[1].size = quarter;
  Song[1].tempo = 3000;
  Song[1].space = 10;
  Song[1].end = 0;

  Song[2].note = B4;
  Song[2].size = dotted;
  Song[2].tempo = 3000;
  Song[2].space = 10;
  Song[2].end = 0;

  Song[3].note = G4;
  Song[3].size = _8th;
  Song[3].tempo = 3000;
  Song[3].space = 10;
  Song[3].end = 0;

  Song[4].note = C5;
  Song[4].size = _8th;
  Song[4].tempo = 3000;
  Song[4].space = 10;
  Song[4].end = 0;

  Song[5].note = B4;
  Song[5].size = _8th;
  Song[5].tempo = 3000;
  Song[5].space = 10;
  Song[5].end = 0;

  Song[6].note = A4;
  Song[6].size = _8th;
  Song[6].tempo = 3000;
  Song[6].space = 10;
  Song[6].end = 0;

  Song[7].note = G4;
  Song[7].size = _8th;
  Song[7].tempo = 3000;
  Song[7].space = 10;
  Song[7].end = 0;

  Song[8].note = A4;
  Song[8].size = _8th;
  Song[8].tempo = 3000;
  Song[8].space = 10;
  Song[8].end = 0;

  Song[9].note = B4;
  Song[9].size = _8th;
  Song[9].tempo = 3000;
  Song[9].space = 10;
  Song[9].end = 0;

  Song[10].note = D4;
  Song[10].size = quarter;
  Song[10].tempo = 3000;
  Song[10].space = 10;
  Song[10].end = 0;

  Song[11].note = D4;
  Song[11].size = quarter;
  Song[11].tempo = 3000;
  Song[11].space = 10;
  Song[11].end = 0;

  Song[12].note = G4;
  Song[12].size = quarter;
  Song[12].tempo = 3000;
  Song[12].space = 10;
  Song[12].end = 0;

  Song[13].note = B4;
  Song[13].size = dotted;
  Song[13].tempo = 3000;
  Song[13].space = 10;
  Song[13].end = 0;

  Song[14].note = G4;
  Song[14].size = _8th;
  Song[14].tempo = 3000;
  Song[14].space = 10;
  Song[14].end = 0;

  Song[15].note = C5;
  Song[15].size = _8th;
  Song[15].tempo = 3000;
  Song[15].space = 10;
  Song[15].end = 0;

  Song[16].note = B4;
    Song[16].size = _8th;
    Song[16].tempo = 3000;
    Song[16].space = 10;
    Song[16].end = 0;

    Song[17].note = A4;
    Song[17].size = _8th;
    Song[17].tempo = 3000;
    Song[17].space = 10;
    Song[17].end = 0;

    Song[18].note = G4;
    Song[18].size = _8th;
    Song[18].tempo = 3000;
    Song[18].space = 10;
    Song[18].end = 0;

    Song[19].note = A4;
    Song[19].size = _8th;
    Song[19].tempo = 3000;
    Song[19].space = 10;
    Song[19].end = 0;

    Song[20].note = B4;
    Song[20].size = _8th;
    Song[20].tempo = 3000;
    Song[20].space = 10;
    Song[20].end = 0;

    Song[21].note = D5;
    Song[21].size = _8th;
    Song[21].tempo = 3000;
    Song[21].space = 10;
    Song[21].end = 0;

    Song[22].note = B4;
    Song[22].size = _8th;
    Song[22].tempo = 3000;
    Song[22].space = 10;
    Song[22].end = 0;

    Song[23].note = C4;
    Song[23].size = quarter;
    Song[23].tempo = 3000;
    Song[23].space = 10;
    Song[23].end = 1;

  Save_Note = Song[0].note;  // Needed for vibrato effect
  INDEX = 0;




  while (1)
  {
	  int i,j;
//	  int i;

	  Message_Pointer = &Message[0];
	  Save_Pointer = &Message[0];
	  Message_Length = sizeof(Message)/sizeof(Message[0]);
	  Delay_msec = 200;
	  Animate_On = 1;




	  while ((GPIOC->IDR & 1 << 10))
	  {
	  for (i=0;i<16;i++)
	  {
	  GPIOD->ODR |= 1 << i;
	  if (i>0)GPIOD->ODR &= ~(1 << (i-1));
	  HAL_Delay(50);
	  }
	  for (i=15;i>=0;i--)
	  	  {
		  GPIOD->ODR |= 1 << i;
	  	  if (i<15)GPIOD->ODR &= ~(1 << (i+1));
	  	  HAL_Delay(50);
	  	  }

	  }
	  INDEX = 0;
	  Music_ON = 1;


	  DIM_Enable = 1;
	  // Green
	  GREEN_BRT = 255;
	  BLUE_BRT = 0;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // Blue
	  GREEN_BRT = 0;
	  BLUE_BRT = 255;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // Red
	  GREEN_BRT = 0;
	  BLUE_BRT = 0;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Magenta
	  GREEN_BRT = 0;
	  BLUE_BRT = 255;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Yellow
	  GREEN_BRT = 255;
	  BLUE_BRT = 0;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Cyan
	  GREEN_BRT = 255;
	  BLUE_BRT = 255;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // White
	  GREEN_BRT = 255;
	  BLUE_BRT = 255;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  DIM_Enable = 0;

	  HAL_Delay(20000);  // Delay to allow song to finish




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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 0;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

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
