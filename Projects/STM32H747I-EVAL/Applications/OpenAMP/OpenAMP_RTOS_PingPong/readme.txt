/**
  @page OpenAMP_RTOS_PingPong OpenAMP Pingpong example

  @verbatim
  ******************************************************************************
  * @file    OpenAMP/OpenAMP_RTOS_PingPong/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the OpenAMP PingPong  Application using FreeRTOS.
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

This application shows How to use OpenAMP MW to create a communication channel (called rpmsg channel) between cores
and send mutual messages in the two directions.
This application is working in mixed mode : CPU1 running with FreeRTOS and CPU2 is running in bare metal mode.

This project contains two workspaces:
OpenAMP MW is dealing with IPC communication.
OpenAMP MW is composed of 2 main components:

    * Resource table definition in rsc_table.c which is allocated in a shared memory (0x38000000)
    * Messaging protocol "rpmsg" provides the messaging infrastructure rely on mailbox and virtio buffers.

OpenAMP MW uses the following HW resources
    * HSEM(Hw Semaphore) peripheral for event signal (mailbox) between CPU1(CM7) and CPU2(CM4).
    * D3-SRAM peripheral for buffer communications (virtio buffers) between CPU1(CM7) and CPU2(CM4)
        Reserved shared memory region for this example: SHM_ADDR=0x38000400 and SHM_SIZE=128k.
        It is defined in platform_info.c file

In this example:
    - System clock configuration is done only once by CPU1(CM7).
    - CPU1(CM7 aka Master processor) wakes up the remote CPU(CM4).
    - CPU1 create application thread  and starts FreeRTOS Kernel.
    - In the thread : Initialize OpenAMP MW which initializes/configures HSEM peripheral through HAL
      and setup openamp-rpmsg framework infrastructure (Master Side).
    - CPU2, after wakeup, initializes OpenAMP MW which initializes/configures HSEM peripheral through HAL
    and setup openamp-rpmsg framework infrastructure(Remote Side).
    - CPU2 creates a communication channel(called rpmsg channel)

    - In a loop, each CPU receives the counter "message", increments it, then sends it to the second CPU.


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note Limitations: the following warnings are thrown when using  OpenAMP-based application:

    + The OpenAMP Library throws the following warnings
     *) ARMCC/ICCARM
      - Middlewares/Third_Party/OpenAMP/open-amp/lib/rpmsg/rpmsg_virtio.c(236): warning:  #111-D: statement is unreachable
               return false;
    *) GCC
     - Middlewares/Third_Party/OpenAMP/open-amp/lib/include/openamp/rpmsg.h:291:2: warning: argument 2 null where non-null expected [-Wnonnull]
     - Middlewares/Third_Party/OpenAMP/libmetal/lib/system/generic/irq.c:43:40: warning: missing braces around initializer [-Wmissing-braces]

    + The application the following warnings
      *) ARMCC
      - ../Common/Src/openamp.c(36): warning:  #1296-D: extended constant initialiser used
           static metal_phys_addr_t shm_physmap[] = { SHM_START_ADDRESS };

      -  ../Common/Src/rsc_table.c(104): warning:  #1296-D: extended constant initialiser used
               .vring0 = {VRING_TX_ADDRESS, VRING_ALIGNMENT, VRING_NUM_BUFFS, VRING0_ID, 0},

      -  ../Common/Src/rsc_table.c(105): warning:  #1296-D: extended constant initialiser used
               .vring1 = {VRING_RX_ADDRESS, VRING_ALIGNMENT, VRING_NUM_BUFFS, VRING1_ID, 0},

      ==> cause:  Syntax to refer symbols defined in the linker files, in C defines in not accepted by the compiler
                  but it doesn't impact the functional aspect.

     *) GCC
       - warning: assignment discards 'volatile' qualifier from pointer target type [-Wdiscarded-qualifiers]
          *table_ptr = &resource_table
       ===> cause: the resource_table is a shared memory between CM4 and CM7 thus it should be volatile to avoid
                   compiler optimizations.

@par Keywords

OpenAMP, multiprocessor, HSEM, Inter processor communication, Communication channel, Mixed mode, RTOS, Baremetal, Message, HSEM, IPC, Mailbox, PingPong

@par Directory contents
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Src/main_cm7.c                     Main CM7 program
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Src/stm32h7xx_it.c                 CM7 Interrupt handlers
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Src/stm32h7xx_hal_timebase_tim.c   TIM HAL timebase implmentation
    - OpenAMP/OpenAMP_RTOS_PingPong/CM4/Src/main_cm4.c                     Main CM4 program
    - OpenAMP/OpenAMP_RTOS_PingPong/CM4/Src/stm32h7xx_it.c                 CM4 Interrupt handlers

    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Src/rsc_table.c                 Resource_table for OpenAMP
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Src/system_stm32h7xx.c          STM32H7xx system configuration file
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Src/mbox_hsem.c                 HSEM mailibox inteface implementation
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Src/openamp.c                   OpenAMP APi wrapper inteface

    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Inc/main.h                         Main CM7 program header file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Inc/FreeRTOS.h                     FreeRTOS configuration file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Inc/stm32h7xx_it.h                 CM7 Interrupt handlers header file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Inc/stm32h7xx_hal_conf.h           CM7 HAL Library Configuration file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM4/Inc/main.h                         Main CM7 program header file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM4/Inc/stm32h7xx_it.h                 CM7 Interrupt handlers header file
    - OpenAMP/OpenAMP_RTOS_PingPong/CM7/Inc/stm32h7xx_hal_conf.h           CM7 HAL Library Configuration file

    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Inc/openamp.h                   User OpenAMP initialization header file
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Inc/rsc_table.h                 Resource_table for OpenAMP header file
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Inc/mbox_hsem.h                 HSEM mailibox inteface header file
    - OpenAMP/OpenAMP_RTOS_PingPong/Common/Inc/openamp_conf.h              openamp configuration header


@par Hardware and Software environment

  - This example runs on STM32H7xx devices.

  - This example has been tested with STM32H7xx_EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H747_CM7_OpenAMP_PingPong and STM32H747_CM4_OpenAMP_PingPong) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot CPU1 (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
