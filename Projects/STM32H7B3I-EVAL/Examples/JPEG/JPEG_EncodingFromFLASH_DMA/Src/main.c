/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the HW JPEG to Encode an RGB image
  *          from FLASH with DMA method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "encode_dma.h"
#include "image_320_240_rgb.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_EncodingFromFLASH_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */
FIL JPEG_File;  /* File object */

JPEG_HandleTypeDef    JPEG_Handle;
uint32_t RGB_ImageAddress;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_BriefDisplay(void);
static void LCD_SizesErrorDisplay(void);
static void LCD_FileErrorDisplay(void);
static void SD_Initialize(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t JpegEncodeProcessing_End = 0;
  uint8_t  lcd_status = BSP_ERROR_NONE;

#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
  RGB_ImageAddress = (uint32_t)Image_ARGB8888;

#elif(JPEG_RGB_FORMAT == JPEG_RGB888)
  RGB_ImageAddress = (uint32_t)Image_RGB888;

#elif(JPEG_RGB_FORMAT == JPEG_RGB565)
  RGB_ImageAddress = (uint32_t)Image_RGB565;

#endif /* JPEG_RGB_FORMAT */


  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

 /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */

  HAL_Init();

  /* Configure the system clock to 280 MHz MHz */
  SystemClock_Config();

/* Initialize the LED1 (set to On when Test OK) */
  BSP_LED_Init(LED1);
/* Initialize the LED2 ( set to On when error) */
  BSP_LED_Init(LED2);


  /*##-1- JPEG Initialization ################################################*/
  /* Init The JPEG Color Look Up Tables used for YCbCr to RGB conversion   */
  JPEG_InitColorTables();

   /* Init the HAL JPEG driver */
  JPEG_Handle.Instance = JPEG;
  HAL_JPEG_Init(&JPEG_Handle);

  /*##-2- LCD Configuration ##################################################*/
  /* Initialize the LCD   */
  lcd_status = BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE);
  if(lcd_status != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /*link board LCD drivers to BASIC GUI LCD drivers*/
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  /* Display example brief   */
  LCD_BriefDisplay();

  /*##-3- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-4- Init the SD Card #################################################*/
    SD_Initialize();

    if(BSP_SD_IsDetected(BSP_SD_INSTANCE))
    {
      /*##-5- Register the file system object to the FatFs module ##############*/
      if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
      {
        /*##-6- Create the JPEG file with write access ########################*/
        if(f_open(&JPEG_File, "image.jpg", FA_CREATE_ALWAYS | FA_WRITE ) == FR_OK)
        {
          /*##-7- JPEG Encoding with DMA (Not Blocking ) Method ################*/
          JPEG_Encode_DMA(&JPEG_Handle, RGB_ImageAddress, RGB_IMAGE_SIZE, &JPEG_File);

          /*##-8- Wait till end of JPEG encoding and perfom Input/Output Processing in BackGround  #*/
          do
          {
            JPEG_EncodeInputHandler(&JPEG_Handle);
            JpegEncodeProcessing_End = JPEG_EncodeOutputHandler(&JPEG_Handle);

          }while(JpegEncodeProcessing_End == 0);

          /*##-9- Close the JPEG file #######################################*/
          f_close(&JPEG_File);

          UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
          UTIL_LCD_SetFont(&Font16);
          UTIL_LCD_DisplayStringAtLine(20,(uint8_t*)" JPEG file encoded and saved on the uSD");

          /* Encoding accomplished successfully */
          BSP_LED_On(LED1);

        }
        else /* Can't Open jpg file for write */
        {
          LCD_FileErrorDisplay();
		  Error_Handler();
        }
      }
      else /* FatFs mount failed */
      {
        Error_Handler();
      }
    }
    else /* uSD not connected  */
    {
      LCD_FileErrorDisplay();
	  Error_Handler();
    }
  }
  else /* FatFs uSD driver link failed */
  {
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
  while(1) {};
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
  while(1) {};
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

/**
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 177, 640, 112,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAtLine(9, (uint8_t *)"   JPEG Encoding from Flash With DMA Method");
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(16, (uint8_t *)"This example shows how to Encode (with DMA model)");
  UTIL_LCD_DisplayStringAtLine(17, (uint8_t *)"and save a JPEG file in uSD");
}

/**
  * @brief  Display error message when Image Sizes do not meet the requirements.
  * @param  None
  * @retval None
  */
static void LCD_SizesErrorDisplay(void)
{
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(27, (uint8_t *)"Input Sizes of the RGB image doesn't meet the requirements");
}

/**  - The LED1 is On if the test ends properly.
  * @brief  Display File access error message.
  * @param  None
  * @retval None
  */
static void LCD_FileErrorDisplay(void)
{
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(27, (uint8_t *)"Unable to create a new jpeg file on the uSD");
  UTIL_LCD_DisplayStringAtLine(28, (uint8_t *)"Check that a valid uSD is connected to CN28 connector");
}

static void SD_Initialize(void)
{
  BSP_SD_Init(BSP_SD_INSTANCE);
}

#ifdef USE_FULL_ASSERT

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
#endif /* USE_FULL_ASSERT */

/**
  * @brief  On Error Handler.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED2);
  while(1)
  {
  }
}

/**
  * @brief  Get the images sizes from BMP header.
  * @param  pInfo : pointer to the Info structure
  * @retval None
  */
void RGB_GetInfo(JPEG_ConfTypeDef *pInfo)
{
  /* Read Images Sizes */
  pInfo->ImageWidth         = RGB_IMAGE_WIDTH;
  pInfo->ImageHeight        = RGB_IMAGE_HEIGHT;

  /* Jpeg Encoding Setting to be setted by users */
  pInfo->ChromaSubsampling  = JPEG_CHROMA_SAMPLING;
  pInfo->ColorSpace         = JPEG_COLOR_SPACE;
  pInfo->ImageQuality       = JPEG_IMAGE_QUALITY;

  /*Check if Image Sizes meets the requirements */
  if (((pInfo->ImageWidth % 8) != 0 ) || ((pInfo->ImageHeight % 8) != 0 ) || \
      (((pInfo->ImageWidth % 16) != 0 ) && (pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE) && (pInfo->ChromaSubsampling != JPEG_444_SUBSAMPLING)) || \
      (((pInfo->ImageHeight % 16) != 0 ) && (pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE) && (pInfo->ChromaSubsampling == JPEG_420_SUBSAMPLING)))
  {
    LCD_SizesErrorDisplay();
    Error_Handler();
  }

}
/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
