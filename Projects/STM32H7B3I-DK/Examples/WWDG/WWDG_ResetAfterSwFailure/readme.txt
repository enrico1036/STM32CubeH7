/**
  @page WWDG_ResetAfterSwFailure Window Watchdog example
  
  @verbatim
  ******************************************************************************
  * @file    WWDG/WWDG_ResetAfterSwFailure/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Window Watchdog Reset After SW failure example.
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

How to perform periodic update of WWDG counter and simulate a software fault that
generates an MCU WWDG reset when a predefined time period has elapsed.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

The WWDG peripheral configuration is ensured by the HAL_WWDG_Init() function.
This later is calling the HAL_WWDG_MspInit() function which core is implementing
the configuration of the needed WWDG resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change WWDG configuration.

The reset scope control bit should be set before enabling the WWDG to ensure
a wide system reset.

The WWDG timeout is set, through counter value, to 53 ms. 
The refresh window is set in order to make user wait 33 ms after a wadchdog refresh, 
before writing again counter. Hence the WWDG counter is refreshed each (33 + 1) ms in the 
main program infinite loop to prevent a WWDG reset. 
LED2 is toggling at same frequency, indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs by pressing the User button (PC.13), the corresponding interrupt is served.

In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the WWDG counter is 
not refreshed).
As a result, when the WWDG counter falls to 63, WWDG reset occurs.

If the WWDG reset is generated, after the system resumes from reset, LED2 is turned ON for 4 seconds.

If the EXTI Line event does not occur, the WWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no WWDG reset.

LED3 is turned ON and remains ON if any error occurs.

@note This example must be tested in standalone mode (not in debug).


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
          - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
          - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
          - The addresses and the size of cacheable buffers (shared between CPU and other masters)
            must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series”

@par Keywords

System, WWDG, Downcounter, MCU Reset, Timeout, Software fault

@par Directory contents 

  - WWDG/WWDG_ResetAfterSwFailure/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - WWDG/WWDG_ResetAfterSwFailure/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_ResetAfterSwFailure/Inc/main.h                  Header for main.c module
  - WWDG/WWDG_ResetAfterSwFailure/Inc/stm32h7b3i_discovery_conf.h STM32H7xx board configuration file
  - WWDG/WWDG_ResetAfterSwFailure/Src/stm32h7xx_it.c          Interrupt handlers
  - WWDG/WWDG_ResetAfterSwFailure/Src/main.c                  Main program
  - WWDG/WWDG_ResetAfterSwFailure/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - WWDG/WWDG_ResetAfterSwFailure/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.
    
  - This example has been tested with STM32H7B3I-DISCO board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 