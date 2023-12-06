/* USER CODE BEGIN Header */
/*
 *  Filename:       main.c
 *  Platform(s):    STM32F401x
 *  Project:        FB19Exa2_Ctrl
 *  Created:        December 06, 2023
 *  Description:    This file implements the main() function of this application.
 *
 *  Glossary:       LED:    Light Emitting Diode
 *                  NEB:    Nucleo Expansion Board
 *                  PB:     Push Button
 *
 *                  Responsibilities
 *                  ================
 *                  This application transmits messages containing the status of
 *                  a single digital input (PB1 on FieldBus19 NEB) as FB19
 *                  Controller using USART2.
 *                  If a STM32 node running the FB19Exa2_Subs application is
 *                  connected to this node, it will switch a single digital
 *                  output on or off (LED1 on the FieldBus19 NEB) depending on
 *                  the state of the digital input. It then returns the state of
 *                  the LED in response messages to this Controller.
 *                  This ping-pong schema is repeated periodically twice per
 *                  second. USART1 is used to display this process on the
 *                  console.
 *
 *                  FB19 Libraries
 *                  ==============
 *                  - FB19CtrlLib
 *
 *                  Configuration
 *                  =============
 *                  FB19 Bitrate: 100kbit/s
 *                  FB19 Subscriber bus address: 1
 *                  FB19 Bus: RS-485 (not RS-232!)
 *                  USART1 RS-232 parameters: 115200/8/N/1
 *                  Used SoC resources:
 *                  - GPIOA, bit 8: FB19Ctrl RS-485 driver Data Output Enable
 *                  - GPIOC, bit 11: Digital input for sensing PB1 state
 *                  - TIM2: FB19Ctrl
 *                  - USART1: RS-232 Terminal
 *                  - USART2: FB19Ctrl
 *                  Hardware:
 *                  - Nucleo Board: Nucleo-64 STM32F401
 *                  - Nucleo Expansion Board for FieldBus19, v2.2 or higher
 *                    - This board contains jumpers that relate to the bus
 *                      system:
 *                      - JP4, JP5, JP6, JP7: connect pins 1 and 2 (RS-485 mode)
 *                      - JP8, JP9: connect pins 2 and 3 (J2 is connected to
 *                        USART6)
 *                    - The board contains furthermore a user push button and a
 *                      user LED
 *
 *  Notes:          The initial version of this file was generated using
 *                  STMicroelectronics STM32CubeMX, v6.8.0.
 *                  Bitcontrol GmbH is grateful for ST's free of charge tools
 *                  and recommends them to any programmer, from hobbyist to
 *                  professional.
 *
 *                  Local symbols and functions that are user code specific
 *                  start with my_ or MY_ respectively for easy spotting.
 *
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> // For printf()

#include "FB19Ctrl.h"
#include "FB19CtrlCfg.h"
#include "FB19LibMsg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MY_BIT_RATE                     100000 // [bit/s]
#define MY_MAIN_LOOP_PERIOD_MS          500    // [ms]
#define MY_MESSAGE_IDENTIFIER           FB19_MSG_ID_USR_LOWEST
#define MY_SUBSCRIBER_BUS_ADDRESS       FB19_BUS_ADDR_SUBS_LOWEST
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
/* These convenience functions capture the different actions. */
static int myStartFB19(void);
static void myCtrlDisplayResponseMsg(void);
static void myCtrlSendToSubs(int payload);
static void myWaitForTick(void);
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
  __HAL_DBGMCU_FREEZE_TIM2();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    HAL_SetTickFreq(HAL_TICK_FREQ_100HZ);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    int status = myStartFB19(); // Start Controller and Subscriber instances
    if (status != R_SUCCESS)
    {
        return 1; // Terminate application, return error value
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    int buttonStatus = 0;
    int rel_time_ms = 0;
    while (1)
    {
        /* Call the FB19 handler functions periodically. */
        FB19Ctrl_handler(&instFB19Ctrl);

        if (rel_time_ms == 0)
        {
            /* Perform domain specific tasks. */
            buttonStatus = GPIOC->IDR & (1 << 11);
            myCtrlSendToSubs(buttonStatus);
        }
        myCtrlDisplayResponseMsg();

        /* Slow this loop down as desired. */
        myWaitForTick(); // Tick frequency is 100Hz; see HAL_SetTickFreq() above
        rel_time_ms += uwTickFreq; // It's actually the period, not the frequency
        rel_time_ms %= MY_MAIN_LOOP_PERIOD_MS;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
    return 0; // If ever reached
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
    setvbuf( stdin, NULL, _IONBF, 0); // Disable console input buffering
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NEB_LED1_GPIO_Port, NEB_LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : NEB_LED1_Pin */
  GPIO_InitStruct.Pin = NEB_LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(NEB_LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : NEB_PB1_Pin */
  GPIO_InitStruct.Pin = NEB_PB1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NEB_PB1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
 * FB19 Controller related: Display the response message from the Subscriber if
 * available.
 */
static void myCtrlDisplayResponseMsg(void)
{
    static uint32_t noiseCharFaultCount;
    static uint32_t rxFaultCount;
    static uint32_t txFaultCount;

    FB19Msg_t msg;
    msg.dscr.msgId = MY_MESSAGE_IDENTIFIER;
    int32_t ii;
    int status;
    uint32_t tmp;

    status = FB19Ctrl_fetch(&instFB19Ctrl, &msg);
    // status equals R_SUCCESS if a response has been received or the response
    // has timed out. In the latter case, the function will also return
    // R_SUCCESS with the error FB19_DRV_ERR_BUS_RSP_FRM_TMO set inside the
    // message.
    if (status == R_SUCCESS)
    {
        switch (msg.dscr.errSts)
        {
        case FB19_DRV_ERR_NONE: // Message received?
            FB19LibMsg_removeInt32(&msg, &ii); // Assume success, ignore return value
            printf("FB19Ctrl rxd msg with val=%li\n", ii);
            break;
        case FB19_DRV_ERR_BUS_RSP_FRM_TMO:
            printf("FB19Ctrl response message timed out\n");
            break;
        default:
            printf("FB19Ctrl rxd msg with error=%i\n", msg.dscr.errSts);
            break;
        }
        printf("\n");
    }

    FB19Ctrl_getNoiseFaultCount(&instFB19Ctrl, &tmp); // Processing return value optional
    if (noiseCharFaultCount != tmp)
    {
        noiseCharFaultCount = tmp;
        printf("FB19Ctrl noise char fault count=%lu\n", noiseCharFaultCount);
    }
    FB19Ctrl_getRxFaultCount(&instFB19Ctrl, &tmp); // Processing return value optional
    if (rxFaultCount != tmp)
    {
        rxFaultCount = tmp;
        printf("FB19Ctrl Rx fault count=%lu\n", rxFaultCount);
    }
    FB19Ctrl_getTxFaultCount(&instFB19Ctrl, &tmp); // Processing return value optional
    if (txFaultCount != tmp)
    {
        txFaultCount = tmp;
        printf("FB19Ctrl Tx fault count=%lu\n", txFaultCount);
    }
}

/*
 * FB19 Controller related: Submit a request message with the provided payload
 * value for transmission to the Subscriber.
 */
static void myCtrlSendToSubs(int payload)
{
    FB19Msg_t msg; // Initialize these fields for avoiding side effects
    msg.dscr.dstAddr = MY_SUBSCRIBER_BUS_ADDRESS;
    msg.dscr.msgId = MY_MESSAGE_IDENTIFIER;
    msg.dscr.pldSize = 0;

    int status;

    FB19LibMsg_appendInt32(&msg, payload); // Assume success, ignore return value
    status = FB19Ctrl_submit(&instFB19Ctrl, &msg);
    if (status == R_SUCCESS)
    {
        printf("FB19Ctrl_submit() succeeded with val=%i\n", payload);
    }
    else
    {
        printf("FB19Ctrl_submit() failed\n");
    }
}

static int myStartFB19(void)
{
    int status;

    printf("FB19Ctrl version: %s\n", FB19Ctrl_getVersion());
    status = FB19Ctrl_start(&instFB19Ctrl, &cfgFB19Ctrl, MY_BIT_RATE);
    if (status == R_SUCCESS)
    {
        printf("FB19Ctrl_start() succeeded\n");
    }
    else
    {
        printf("FB19Ctrl_start() failed\n");
    }
    printf("\n");

    return status;
}

static void myWaitForTick(void)
{
    volatile uint32_t tickCnt = HAL_GetTick();
    while (tickCnt == HAL_GetTick());
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
