/**
  @page RNG_GenerateRandomNumbers RNG : Random Number Generation
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RNG/RNG_GenerateRandomNumbers/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RNG_GenerateRandomNumbers example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Configuration of the RNG to generate 32-bit long random numbers. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H723xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

Example execution:
After startup from reset and system configuration, RNG configuration is performed.
(Configure PLLSAI1 to enable 48M domain, then enable PLLSAI1 output mapped on 48MHz domain clock
and configure PLLSAI as RNG clock source).

User is then asked to press User push-button (LED1 blinking fast).
On User push-button press, several (8) Random 32bit numbers are generated
(DRDY flag is polled until 1, indicating a random number has been generated and could be retrieved from DR register).
Corresponding generated values are available and stored in a u32 array (aRandom32bit), 
whose content could be displayed using debugger (Watch or LiveWatch features).
After successful Random numbers generation, LED1 is turned On. 
In case of errors, LED1 is slowly blinking (1sec period).


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

@par Directory contents 

  - RNG/RNG_GenerateRandomNumbers/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RNG/RNG_GenerateRandomNumbers/Inc/main.h                  Header for main.c module
  - RNG/RNG_GenerateRandomNumbers/Inc/stm32_assert.h          Template file to include assert_failed function
  - RNG/RNG_GenerateRandomNumbers/Src/stm32h7xx_it.c          Interrupt handlers
  - RNG/RNG_GenerateRandomNumbers/Src/main.c                  Main program
  - RNG/RNG_GenerateRandomNumbers/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Push User push-button and use Variable watch window from debugger to access to values of generated numbers.
   (A break point could be set on LED_On() call, at end of RandomNumbersGeneration() function).

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
