/**
  @page HSEM_ReadLock HSEM ReadLock example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    HSEM/HSEM_ReadLock/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HSEM ReadLock example.
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
  @endverbatim

@par Example Description

How to enable, take then release semaphore using 2 different Processes.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

First, Enable HW semaphore using __HAL_RCC_HSEM_CLK_ENABLE macro.
Secondly, Semaphore 9 with Process1(ID=12)is locked in 2-step(Write) lock procedure.
    ==> Green LED (LED1)is ON, when lock with Process1 is done correctly. 
Third, start new take of the same semaphore 9  with Process1(ID 13)via the same semaphore 9 using 2-step(Write) lock procedure,
    ==> No lock is done, because the semaphore 9 is already locked. otherwise red LED is ON.
Finally, semaphore 9 with Process1(ID=12) is released and take the same semaphore 9 with Process2(ID=13)
    ==> Check take status, if equal to 0, release done otherwise, red LED is ON.
    ==> Blue LED (LED2)is ON, when lock with Process2 is done correctly.  
In every step, if any mismatch with right result,red LED(LED3) is ON.

In this example, HCLK is configured at 400 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
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

Hardware semaphore, Semaphore, HSEM, Lock, Unlock, Take, Release, Process, LED

@par Directory contents 

  - HSEM/HSEM_ReadLock/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HSEM/HSEM_ReadLock/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HSEM/HSEM_ReadLock/Inc/main.h                  Header for main.c module  
  - HSEM/HSEM_ReadLock/Src/stm32h7xx_it.c          Interrupt handlers
  - HSEM/HSEM_ReadLock/Src/main.c                  Main program
  - HSEM/HSEM_ReadLock/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
