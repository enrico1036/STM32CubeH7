/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "stm32h747i_eval.h"
#include "stm32h747i_eval_io.h"
//#include "lcd_log.h"
#include "usbh_msc.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
#include "lcd_trace.h"
#include "stm32_lcd.h"
#include "utilities_conf.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLI_HID_IDLE = 0,
  APPLI_HID_WAIT,
  APPLI_HID_START,
  APPLI_HID_MOUSE,
  APPLI_HID_KEYBOARD,
}Demo_HID_State;

typedef enum {
  HID_MOUSE_IDLE = 0,
  HID_MOUSE_WAIT,
  HID_MOUSE_START,
}Demo_mouse_State;

typedef enum {
  HID_KEYBOARD_IDLE = 0,
  HID_KEYBOARD_WAIT,
  HID_KEYBOARD_START,
}Demo_keyboard_State;

typedef enum {
  DEMO_IDLE = 0,
  DEMO_WAIT,
  DEMO_MSC_START,
  DEMO_HID_START,
  DEMO_HID,
  DEMO_MSC,
  DEMO_MSC_REENUMERATE,
  DEMO_HID_REENUMERATE,
}Demo_State;

typedef enum {
  APPLI_MSC_IDLE = 0,
  APPLI_MSC_WAIT,
  APPLI_MSC_FILE_OPERATIONS,
  APPLI_MSC_EXPLORER,
}Demo_MSC_State;

typedef struct _DemoStateMachine {
  __IO Demo_State           state;
  __IO Demo_MSC_State       msc_state;
  __IO Demo_HID_State       hid_state;
  __IO Demo_mouse_State     mouse_state;
  __IO Demo_keyboard_State  keyboard_state;
  __IO uint8_t              select;
}DEMO_StateMachine;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_FS_DISCONNECT,
  APPLICATION_FS_START,
  APPLICATION_FS_READY,
  APPLICATION_HS_DISCONNECT,
  APPLICATION_HS_START,
  APPLICATION_HS_READY,
  APPLICATION_RUNNING,
}DUAL_ApplicationTypeDef;

extern DEMO_StateMachine demo;
extern USBH_HandleTypeDef hUSBHost_FS;
extern USBH_HandleTypeDef hUSBHost_HS;
extern HCD_HandleTypeDef hhcd_FS;
extern HCD_HandleTypeDef hhcd_HS;
extern FATFS USBH_fatfs;
extern DUAL_ApplicationTypeDef Appli_HS_state;
extern DUAL_ApplicationTypeDef Appli_FS_state;
extern uint8_t *DEMO_MOUSE_menu[];
extern uint8_t prev_select;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LCD_UsrLogY(...)                   LCD_LineColor = LCD_COLOR_YELLOW;\
                                           printf(__VA_ARGS__);\

/* Exported functions ------------------------------------------------------- */
FRESULT Explore_Disk(char *path, uint8_t recu_level);
void MSC_File_Operations(void);
void Menu_Init(void);
void MSC_MenuProcess(void);
void HID_MenuProcess(void);
void Demo_SelectItem(uint8_t **menu, uint8_t item);
void HID_MouseMenuProcess(void);
void HID_KeyboardMenuProcess(void);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed(uint8_t button_idx);
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);
void USR_KEYBRD_ProcessData(uint8_t data);
void DUAL_MenuProcess(void);
void DUAL_MenuInit(void);
void Demo_ProbeKey(uint32_t state);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
