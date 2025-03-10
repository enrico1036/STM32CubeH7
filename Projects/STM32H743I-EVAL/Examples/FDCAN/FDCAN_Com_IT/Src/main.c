/**
  ******************************************************************************
  * @file    FDCAN_Com_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup FDCAN_Com_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan;
FDCAN_FilterTypeDef sFilterConfig;
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[16];
FDCAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};
__IO uint32_t Notification_flag = 0;
__IO uint32_t Transmission_flag = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static uint32_t BufferCmp8b(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Configure Tamper push-button in interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /*                Bit time configuration:
    Bit time parameter         | Nominal      |  Data
    ---------------------------|--------------|----------------
    fdcan_ker_ck               | 20 MHz       | 20 MHz
    Time_quantum (tq)          | 50 ns        | 50 ns
    Synchronization_segment    | 1 tq         | 1 tq
    Propagation_segment        | 23 tq        | 1 tq
    Phase_segment_1            | 8 tq         | 4 tq
    Phase_segment_2            | 8 tq         | 4 tq
    Synchronization_Jump_width | 8 tq         | 4 tq
    Bit_length                 | 40 tq = 2 �s | 10 tq = 0.5 �s
    Bit_rate                   | 0.5 MBit/s   | 2 MBit/s
  */
  hfdcan.Instance = FDCANx;
  hfdcan.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
  hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission = ENABLE;
  hfdcan.Init.TransmitPause = DISABLE;
  hfdcan.Init.ProtocolException = ENABLE;
  hfdcan.Init.NominalPrescaler = 0x1; /* tq = NominalPrescaler x (1/fdcan_ker_ck) */
  hfdcan.Init.NominalSyncJumpWidth = 0x8;
  hfdcan.Init.NominalTimeSeg1 = 0x1F; /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.NominalTimeSeg2 = 0x8;
  hfdcan.Init.DataPrescaler = 0x1;
  hfdcan.Init.DataSyncJumpWidth = 0x4;
  hfdcan.Init.DataTimeSeg1 = 0x5; /* DataTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.DataTimeSeg2 = 0x4;
  hfdcan.Init.MessageRAMOffset = 0;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo0ElmtsNbr = 2;
  hfdcan.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.RxBuffersNbr = 0;
  hfdcan.Init.TxEventsNbr = 0;
  hfdcan.Init.TxBuffersNbr = 0;
  hfdcan.Init.TxFifoQueueElmtsNbr = 2;
  hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  HAL_FDCAN_Init(&hfdcan);

  /* Configure Rx filter */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x111;
  sFilterConfig.FilterID2 = 0x7FF; /* For acceptance, MessageID and FilterID1 must match exactly */
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);

  /* Configure global filter to reject all non-matching frames */
  HAL_FDCAN_ConfigGlobalFilter(&hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

  /* Configure Rx FIFO 0 watermark to 2 */
  HAL_FDCAN_ConfigFifoWatermark(&hfdcan, FDCAN_CFG_RX_FIFO0, 2);

  /* Activate Rx FIFO 0 watermark notification */
  HAL_FDCAN_ActivateNotification(&hfdcan, FDCAN_IT_RX_FIFO0_WATERMARK, 0);

  /* Prepare Tx Header */
  TxHeader.Identifier = 0x111;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_ON;
  TxHeader.FDFormat = FDCAN_FD_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;

  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);

  /* Infinite loop */
  while (1)
  {
    /* check on the transmission flag */
    if (Transmission_flag == 1)
    {
      BSP_LED_Off(LED1);

      /* Add messages to TX FIFO */
      HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, &TxData[0]);
      HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, &TxData[8]);

      HAL_Delay(5);

      Transmission_flag = 0;
    }

    /* check on the watermark notification flag */
    if (Notification_flag == 1)
    {
      /* Retrieve Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &RxData[8]);

      /* Compare payload to expected data */
      if (BufferCmp8b(TxData, RxData, 16) != 0)
      {
        Error_Handler();
      }
      else
      {
        BSP_LED_On(LED1);
      }

      Notification_flag = 0;
    }
  }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_WATERMARK) != RESET)
  {
    Notification_flag = 1;
  }
}

/**
  * @brief Compares two buffers.
  * @par Input
  *  - pBuffer1, pBuffer2: buffers to be compared.
  *  - BufferLength: buffer's length
  * @par Output
  * None.
  * @retval
  *   0: pBuffer1 identical to pBuffer2
  *   1: pBuffer1 differs from pBuffer2
  */
static uint32_t BufferCmp8b(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

/**
  * @brief EXTI line detection callback
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_TAMPER_PIN)
  {
    Transmission_flag = 1;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 40
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 40;  /* fdcan_ker_ck = 20 MHz */

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; /* fdcan_pclk = 100 MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED3);

  while(1)
  {
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
