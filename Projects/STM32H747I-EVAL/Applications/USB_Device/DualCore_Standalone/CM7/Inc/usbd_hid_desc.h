/**
  ******************************************************************************
  * @file    USB_Device/DualCore_Standalone/Inc/usbd_hid_desc.h
  * @author  MCD Application Team
  * @brief   Header for usbd_hid_desc.c module
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
#ifndef __USBD_HID_DESC_H
#define __USBD_HID_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define         DEVICE_ID1          (0x5C001000)
#define         DEVICE_ID2          (0x5C001004)
#define         DEVICE_ID3          (0x5C001008)

#define  USB_SIZ_STRING_SERIAL       0x1A

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_DescriptorsTypeDef HID_Desc;

#endif /* __USBD_HID_DESC_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
