/**
  @page FDCAN_Clock_calibration FDCAN Clock Calibration example

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    FDCAN/FDCAN_Clock_calibration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Clock_calibration example.
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

This example shows how to achieve clock calibration on an FDCAN unit.

Boards: STM32H743I-EVAL (embeding an STM32H743xx device)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |FDCAN1        |                     |        FDCAN1|          |
  |          |              |                     |              |          |
  |          |       CAN    |_____________________|    CAN       |          |
  |          |    connector |                     | connector    |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |   _    _           _    |                     |   _    _                |
  |  |_|  |_|         (_)   |                     |  |_|  |_|               |
  | LED3  LED1      Tamper  |                     | LED3  LED1              |
  |                         |                     |                         |
  |      STM32 Board 1      |                     |      STM32 Board 2      |
  |        Receiver         |                     |       Transmitter       |
  |   (To be calibrated)    |                     |      (Calibrator)       |
  |_________________________|                     |_________________________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

Then:
 - Clock calibration is activated in Board 1 which constrains the FDCAN module
   to automatically enter Restricted Operation Mode.
 - FDCAN module in Board 2 is started and continuously sending calibration
   messages.

When Tamper push-button is pressed in Board 1, its FDCAN module is started and
use the received calibration messages to achieve basic calibration and then
precision calibration.

Finally, when precision calibration state is reached, FDCAN module in Board 1 exits Restricted Operation Mode and sends
validation message with ID 0x111 to Board 2 which then stops sending calibration messages and sends back a confirmation
message with ID 0x123 and with the same data content of Board 1's validation message. Board 1 receives the confirmation
message from Board 2 and compare it to expected data.

At the end of this process, LED1 should blink on both Boards.
If at any time of the process an error is encountered LED3 is turned ON.

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

FDCAN, Calibration, Message, Transmit, Receive, Button

@par Directory contents 

  - FDCAN/FDCAN_Clock_calibration/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Clock_calibration/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - FDCAN/FDCAN_Clock_calibration/Inc/main.h                  Header for main.c module  
  - FDCAN/FDCAN_Clock_calibration/Src/stm32h7xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Clock_calibration/Src/main.c                  Main program
  - FDCAN/FDCAN_Clock_calibration/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Clock_calibration/Src/system_stm32h7xx.c      stm32h7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with a couple of STM32H743I-EVAL boards embedding a 
    STM32H743xx device and can be easily tailored to any other supported device 
    and development board.
    
  - STM32H743I-EVAL set-up
    - Connect a CAN cable to the CAN connectors of the two EVAL boards (CN3)
    - Jumpers JP1 and JP2 => fitted
    - Solder bridges SB59 and SB60 => open
    - Solder bridge SB50 => closed

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Uncomment the "#define FDCAN_receiver" into main.c (default configuration)
 - Rebuild all files and load your image into Board 1 target memory
 - Comment the "#define FDCAN_receiver" into main.c
 - Rebuild all files and load your image into Board 2 target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
