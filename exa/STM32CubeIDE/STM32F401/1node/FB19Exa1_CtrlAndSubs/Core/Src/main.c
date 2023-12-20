/* USER CODE BEGIN Header */
/*
 *  Filename:       main.c
 *  Platform(s):    STM32F401x
 *  Project:        FB19Exa1_CtrlAndSubs
 *  Created:        December 05, 2023
 *  Description:    This file implements the main() function of this application.
 *
 *  Glossary:       LED:    Light Emitting Diode
 *                  NEB:    Nucleo Expansion Board for RS-232 & RS-485
 *                          communication by Bitcontrol GmbH, Switzerland,
 *                          v2.2 or higher
 *                  PB:     Push Button
 *
 *                  Responsibilities
 *                  ================
 *                  This application transmits messages containing the status of
 *                  a single digital input (PB1 on the NEB) as FB19 Controller
 *                  using USART2, and receives them as FB19 Subscriber using
 *                  USART6.
 *
 *                  The Subscriber switches a single digital output on or off
 *                  (LED1 on the NEB) depending on the state of the digital
 *                  input. If the number is 0, the LED is switched off,
 *                  otherwise it is switched on.
 *
 *                  It then returns the number to the FB19 Controller.
 *                  This ping-pong schema is repeated periodically twice per
 *                  second.
 *                  USART1 is used to display this process on a serial terminal.
 *
 *                  FB19 Libraries
 *                  ==============
 *                  - FB19CtrlLib
 *                  - FB19SubsLib
 *
 *                  Configuration
 *                  =============
 *                  FB19 Bitrate: 100kbit/s
 *                  FB19 Controller bus address: 0 (fix per specification)
 *                  FB19 Subscriber bus address: 1
 *                  FB19 Bus: RS-485 (not RS-232!)
 *                  USART1 RS-232 parameters: 115200/8/N/1
 *                  Used SoC resources:
 *                  - GPIOA, bit 7: FB19Subs RS-485 driver Data Output Enable
 *                  - GPIOA, bit 8: FB19Ctrl RS-485 driver Data Output Enable
 *                  - GPIOC, bit 10: Digital output for setting LED1 state
 *                  - GPIOC, bit 11: Digital input for sensing PB1 state
 *                  - TIM2: FB19Ctrl
 *                  - TIM3: FB19Subs
 *                  - USART1 (UART1): RS-232 Terminal
 *                  - USART2 (UART2): FB19Ctrl (observe required solder bridge
 *                    changes on Nucleo board!)
 *                  - USART6 (UART3): FB19Subs
 *                  Hardware:
 *                  - Nucleo Board: Nucleo-64 STM32F401
 *                    - Solder bridge settings different from default:
 *                      SB13: Off, SB14: Off, SB61: Off, SB62: On, SB63: On
 *                  - Nucleo Expansion Board for RS-232 & RS-485 communication
 *                    - This board contains jumpers that relate to the bus
 *                      system:
 *                      - JP4, JP5, JP6, JP7: Connect pins 1 & 2
 *                        This selects RS-485 for UART 2 and UART3
 *                      - JP8, JP9: Connect pins 2 & 3
 *                        This connects UART3 to J2
 *
 *                  Fault Behavior
 *                  ==============
 *                  Assumptions:
 *                  - All required jumpers are at their position (see above)
 *                  - The loopback cable that connects J1 with J2 of the NEB
 *                    is in place
 *                  If the assumptions above are met, the removal of one or
 *                  more jumpers has the effects as described below.
 *                  When the jumpers are re-inserted, the bus system will
 *                  resume normal operation immediately.
 *                  For details of the jumper wiring see the electrical
 *                  diagram.
 *                  Jumper removal:
 *
 *                  - JP4 and JP5 for USART2 (Bus Controller):
 *                    - JP4 (Rx) is removed, JP5 stays inserted (disconnect
 *                      Bus Ctrl UART Rx signal from transceiver data output
 *                      pin):
 *                      The PB1 -> LED1 functionality works. Reason:
 *                      The messages are successfully submitted to the Bus
 *                      Controller (FB19Ctrl_submit() returns R_SUCCESS) and
 *                      sent to the Bus Subscriber, which returns the expected
 *                      response frame. However, the 'Rx listens Tx' feature
 *                      detects the missing characters during transmission and
 *                      flags a frame transmission error (error 0x01 -
 *                      FB19_DRV_ERR_BUS_FRM_TX) in the response message.
 *                    - JP5 (Tx) is removed, JP4 stays inserted (disconnect
 *                      Bus Ctrl UART Tx signal from its transceiver data input
 *                      pin):
 *                      The PB1 -> LED1 functionality doesn't work. Reason:
 *                      The messages are successfully submitted to the Bus
 *                      Controller (FB19Ctrl_submit() returns R_SUCCESS), but
 *                      they never reach the Bus Subscriber since the transmit
 *                      data signal to the transceiver is interrupted. The
 *                      Bus Controller flags a frame transmission error
 *                      (error 0x01 - FB19_DRV_ERR_BUS_FRM_TX) in the response
 *                      message.
 *                    - JP4 (Rx) and JP5 (Tx) are removed:
 *                      The behavior is identical to the case where JP5 is
 *                      removed and JP4 stays inserted.
 *
 *                  - JP6 and JP7 for USART6 (Bus Subscriber):
 *                    - JP6 (Rx) is removed, JP7 stays inserted (disconnect
 *                      Bus Subs UART Rx signal from transceiver data output
 *                      pin):
 *                      The PB1 -> LED1 functionality doesn't work. Reason:
 *                      The messages are successfully submitted to the Bus
 *                      Controller (FB19Ctrl_submit() returns R_SUCCESS) and
 *                      sent to the Bus Subscriber. They don't arrive there
 *                      however since the receive data signal from the
 *                      transceiver is interrupted. The Bus Controller
 *                      subsequently flags a response timeout error (0x80 -
 *                      FB19_DRV_ERR_BUS_RSP_FRM_TMO) in the response message.
 *                    - JP7 (Tx) is removed, JP6 stays inserted (disconnect
 *                      Bus Subs UART Tx signal from its transceiver data input
 *                      pin):
 *                      The PB1 -> LED1 functionality works. Reason:
 *                      The messages are successfully submitted to the Bus
 *                      Controller (FB19Ctrl_submit() returns R_SUCCESS) and
 *                      sent to the Bus Subscriber. The subscriber processes
 *                      the messages and transmits the results back to the Bus
 *                      Controller. They don't arrive there however since the
 *                      transmit data signal to the transceiver is interrupted;
 *                      the frame doesn't reach the transceiver.
 *                      The Bus Controller subsequently flags a response timeout
 *                      error (0x80 - FB19_DRV_ERR_BUS_RSP_FRM_TMO) in the
 *                      response message.
 *                    - JP6 (Rx) and JP7 (Tx) are removed:
 *                      The behavior is identical to the case where JP6 is
 *                      removed and JP7 stays inserted.
 *
 *                  - JP8 and JP9 for the connector J2, which is assigned to the
 *                    FB19 Subscriber:
 *                    - JP8 is removed, JP9 stays inserted:
 *                      The RS-485 positive line is disconnected from its driver
 *                      (Rx and Tx); since the negative line is still connected
 *                      via JP9, the RS-485 bus still works on short lines.
 *                      Thus this action shows no effects.
 *                    - JP9 is removed, JP8 stays inserted:
 *                      The RS-485 negative line is disconnected from its driver
 *                      (Rx and Tx); since the positive line is still connected
 *                      via JP8, the RS-485 bus still works on short lines.
 *                      Thus this action shows no effects.
 *                    - JP8 and JP9 are removed:
 *                      Both RS-485 signal lines are disconnected from their
 *                      driver; this disconnects the Bus Subscriber from the
 *                      RS-485 bus.
 *                      Thus the Bus Controller transmits messages to the bus,
 *                      but doesn't receive responses from the FB19 Subscriber.
 *                      The Bus Controller subsequently flags a response timeout
 *                      error (0x80 - FB19_DRV_ERR_BUS_RSP_FRM_TMO) in the
 *                      response message.
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
#include "FB19Subs.h"
#include "FB19SubsCfg.h"
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
static void myCtrlSendToSubs(BOOL  payload);
static void mySubsProcessMsg(void); // Comprises fetch, process, submit
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
    __HAL_DBGMCU_FREEZE_TIM3();
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
    BOOL buttonPressed = FALSE;
    int rel_time_ms = 0;
    while (1)
    {
        /* Call the FB19 handler functions periodically. */
        FB19Ctrl_handler(&myInstFB19Ctrl);
        FB19Subs_handler(&myInstFB19Subs);

        if (rel_time_ms == 0)
        {
            /* Perform domain specific tasks. */
            buttonPressed = GPIOC->IDR & (1 << 11) ? FALSE : TRUE; // 3.3V when NOT pressed
            myCtrlSendToSubs(buttonPressed);
        }
        mySubsProcessMsg(); // Fetch msg from Rx queue, process it, submit result to Tx queue
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
    setvbuf(stdin, NULL, _IONBF, 0); // Disable console input buffering
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

    int32_t buttonPressed;
    FB19Msg_t msg;
    msg.dscr.msgId = MY_MESSAGE_IDENTIFIER;
    int status;
    uint32_t tmp;

    status = FB19Ctrl_fetch(&myInstFB19Ctrl, &msg);
    // status equals R_SUCCESS if a response has been received or the response
    // has timed out. In the latter case, the error FB19_DRV_ERR_BUS_RSP_FRM_TMO
    // is set inside the message.
    if (status == R_SUCCESS)
    {
        switch (msg.dscr.errSts)
        {
        case FB19_DRV_ERR_NONE: // Message received?
            FB19LibMsg_removeInt32(&msg, &buttonPressed); // Assume success, ignore return value
            printf("FB19Ctrl rxd msg with val=%li\n", buttonPressed);
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

    /* Display error counts if they have changed. */
    FB19Ctrl_getNoiseFaultCount(&myInstFB19Ctrl, &tmp); // Processing return value optional
    if (noiseCharFaultCount != tmp)
    {
        noiseCharFaultCount = tmp;
        printf("FB19Ctrl noise char fault count=%lu\n", noiseCharFaultCount);
    }
    FB19Ctrl_getRxFaultCount(&myInstFB19Ctrl, &tmp); // Processing return value optional
    if (rxFaultCount != tmp)
    {
        rxFaultCount = tmp;
        printf("FB19Ctrl Rx fault count=%lu\n", rxFaultCount);
    }
    FB19Ctrl_getTxFaultCount(&myInstFB19Ctrl, &tmp); // Processing return value optional
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
static void myCtrlSendToSubs(BOOL payload)
{
    FB19Msg_t msg; // Initialize these fields for avoiding side effects
    msg.dscr.dstAddr = MY_SUBSCRIBER_BUS_ADDRESS;
    msg.dscr.msgId = MY_MESSAGE_IDENTIFIER;
    msg.dscr.pldSize = 0;

    int status;

    FB19LibMsg_appendInt32(&msg, payload); // Assume success, ignore return value
    status = FB19Ctrl_submit(&myInstFB19Ctrl, &msg);
    if (status == R_SUCCESS)
    {
        printf("FB19Ctrl_submit() succeeded with val=%i\n", payload);
    }
    else
    {
        printf("FB19Ctrl_submit() failed\n");
    }
}

/*
 * FB19 Subscriber related: Fetch a message from the Rx queue if available,
 * switch the LED1 on or off depending on the message contents, and return
 * a response message with the LED1 status to the FB19 Controller.
 */
static void mySubsProcessMsg(void)
{
    static uint32_t noiseFaultCount;
    static uint32_t rxFaultCount;
    static uint32_t txFaultCount;

    int32_t buttonPressed;
    FB19Msg_t msg;
    msg.dscr.msgId = MY_MESSAGE_IDENTIFIER;
    int status;
    uint32_t tmp;

    status = FB19Subs_fetch(&myInstFB19Subs, &msg);
    // status equals R_ERROR unless there is a message with a matching
    // identifier in the receive queue.
    if (status == R_SUCCESS) // Message received?
    {
        if (msg.dscr.errSts == FB19_DRV_ERR_NONE)
        {
            status = FB19LibMsg_removeInt32(&msg, &buttonPressed);
            if (status == R_SUCCESS)
            {
                if (buttonPressed)
                {
                    GPIOC->ODR &= ~(1 << 10); // Switch LED1 on
                }
                else
                {
                    GPIOC->ODR |= (1 << 10); // Switch LED1 off
                }

                printf("FB19Subs rxd msg with val=%li\n", buttonPressed);
                msg.dscr.dstAddr = FB19_BUS_ADDR_CTRL;
                FB19LibMsg_appendInt32(&msg, buttonPressed); // Assume success, ignore return value
                status = FB19Subs_submit(&myInstFB19Subs, &msg);
                if (status == R_SUCCESS)
                {
                    printf("FB19Subs txd msg with val=%li\n", buttonPressed);
                }
                else
                {
                    printf("FB19Subs_submit() failed\n");
                }
            }
            else
            {
                printf("FB19LibMsg_removeInt32() failed\n");
            }
        }
        else
        {
            printf("FB19Subs rxd msg with error=%i\n", msg.dscr.errSts);
        }
    }

    /* Display error counts if they have changed. */
    FB19Subs_getNoiseFaultCount(&myInstFB19Subs, &tmp); // Processing return value is optional
    if (noiseFaultCount != tmp)
    {
        noiseFaultCount = tmp;
        printf("FB19Subs noise fault count=%lu\n", noiseFaultCount);
    }
    FB19Subs_getRxFaultCount(&myInstFB19Subs, &tmp); // Processing return value is optional
    if (rxFaultCount != tmp)
    {
        rxFaultCount = tmp;
        printf("FB19Subs Rx fault count=%lu\n", rxFaultCount);
    }
    FB19Subs_getTxFaultCount(&myInstFB19Subs, &tmp); // Processing return value is optional
    if (txFaultCount != tmp)
    {
        txFaultCount = tmp;
        printf("FB19Subs Tx fault count=%lu\n", txFaultCount);
    }
}

static int myStartFB19(void)
{
    int status;

    printf("FB19Ctrl version: %s\n", FB19Ctrl_getVersion());
    status = FB19Ctrl_start(&myInstFB19Ctrl, &myCfgFB19Ctrl, MY_BIT_RATE);
    if (status == R_SUCCESS)
    {
        printf("FB19Ctrl_start() succeeded\n");
    }
    else
    {
        printf("FB19Ctrl_start() failed\n");
    }
    printf("\n");

    if (status == R_SUCCESS)
    {
        printf("FB19Subs version: %s\n", FB19Subs_getVersion());
        status = FB19Subs_start(&myInstFB19Subs, &myCfgFB19Subs, MY_BIT_RATE);
        if (status == R_SUCCESS)
        {
            printf("FB19Subs_start() succeeded\n");
        }
        else
        {
            printf("FB19Subs_start() failed\n");
        }
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
