﻿/**
  @page STemWin_memory_device STemWin memory device Readme file

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    STemWin/STemWin_memory_device/CM7/readme.txt
  * @author  MCD Application Team
  * @brief   Description of STemWin memory device application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This directory contains a set of source files that implement a simple "memory device"
application based on STemWin for STM32H7xx devices.

The application shows the capability of STemWin to use the memory devices to achieve tricky operations
like rotations, shifts and zoom.

The 4 leds are blinking to show that the program is runing.

 How to interact with the application
 ------------------------------------
 - The application is an automatic run.
 - It will show an alternance between rotating, zooming, shifting of different wheels
 - It will loop infinetly.

 At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)

System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note For the Cortex-M7, If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Graphics, Display, STemWin, LTDC, Memory, Operations, Rotation, Shift, Zoom.

@par Directory contents

    - STemWin/STemWin_memory_device/CM7/Core/Inc/main.h			        Main program header file
    - STemWin/STemWin_memory_device/CM7/Core/Inc/STM32h7xx_hal_conf.h	Library Configuration file
    - STemWin/STemWin_memory_device/CM7/Core/Inc/STM32h7xx_it.h		    Interrupt handlers header file
    - STemWin/STemWin_memory_device/CM7/Core/Src/main.c			        Main program file
    - STemWin/STemWin_memory_device/CM7/Core/Src/STM32h7xx_it.c		    STM32H7xx Interrupt handlers
    - STemWin/STemWin_memory_device/CM7/Core/Src/system_STM32h7xx.c		STM32H7xx system file
    - STemWin/STemWin_memory_device/CM7/STemWin/App/generated/Wheel2.c	Wheel2 picture
    - STemWin/STemWin_memory_device/CM7/STemWin/App/generated/Wheel3.c	Wheel3 picture
    - STemWin/STemWin_memory_device/CM7/STemWin/App/generated/Wheel4.c	Wheel4 picture
    - STemWin/STemWin_memory_device/CM7/STemWin/App/generated/Wheel5.c	Wheel5 picture
    - STemWin/STemWin_memory_device/CM7/STemWin/App/generated/garage.c 	garage picture
    - STemWin/STemWin_memory_device/CM7/STemWin/App/memory_device_app.c memory devce application
    - STemWin/STemWin_memory_device/CM7/STemWin/Target/GUIConf.c		Display controller initialization
    - STemWin/STemWin_memory_device/CM7/STemWin/Target/GUIConf.h		Header for GUIConf.c
    - STemWin/STemWin_memory_device/CM7/STemWin/Target/LCDConf.c		Configuration file for the GUI library
    - STemWin/STemWin_memory_device/CM7/STemWin/Target/LCDConf.h		Header for LCDConf.c

    - STemWin/STemWin_memory_device/CM4/Inc/stm32h7xx_hal_conf.h       	HAL configuration file for Cortex-M4
    - STemWin/STemWin_memory_device/CM4/Inc/stm32h7xx_it.h          	Interrupt handlers header file for Cortex-M4
    - STemWin/STemWin_memory_device/CM4/Inc/main.h                  	Main configuration file for Cortex-M4
    - STemWin/STemWin_memory_device/CM4/Src/stm32h7xx_it.c          	Interrupt handlers for Cortex-M4
    - STemWin/STemWin_memory_device/CM4/Src/main.c                  	Main program  for Cortex-M4

@par Hardware and Software environment

  - This application runs on STM32H747xx devices.

  - This application has been tested with STMicroelectronics STM32H747I-DISCO
    boards and can be easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
  - Open your preferred toolchain
 - For CM4 target configuration (STM32H747I-Discovery_CM4) :
     - Rebuild all files
     - Load image into target memory

 - For CM7 target configuration :
  - Rebuild all files and load your image into target memory
  - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
