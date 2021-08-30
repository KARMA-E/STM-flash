/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v2.0_Cube
  * @brief          : Memory management layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "flash.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */
#ifdef STORAGE_LUN_NBR
	#warning undef system defines

	#undef STORAGE_LUN_NBR
	#undef STORAGE_BLK_NBR
	#undef STORAGE_BLK_SIZ

	#define STORAGE_LUN_NBR                  1
	#define STORAGE_BLK_NBR                  FL_PAGE_NBR
	#define STORAGE_BLK_SIZ                  FL_PAGE_SIZ
#endif
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'K', 'A', 'R', 'M', 'A', '-', 'E', ' ', /* Manufacturer : 8 bytes */
  'F', 'P', 'a', 's', 's', '-', '1', 'v', /* Product      : 16 Bytes */
  '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '1', 'v', '0' ,' '                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes over USB FS IP
  * @param  lun:
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  block_num: .
  * @param  block_size: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
	if (FLASH->SR & FLASH_SR_BSY)
	{
		return (USBD_BUSY);
	}

	return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */

	uint32_t _cur_addr = 0;
	uint32_t _cur_word = 0;

	for(uint32_t blk_num = 0; blk_num < blk_len; blk_num++)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		GPIOA->CRH ^= GPIO_CRH_MODE9_0;
		GPIOA->BSRR = GPIO_BSRR_BR9;

		if(blk_num + blk_addr >= ((FL_END_ADDR_REAL - FL_START_ADDR) / FL_PAGE_SIZ))
		{
			blk_addr = ((FL_END_ADDR_REAL - FL_START_ADDR) / FL_PAGE_SIZ) - 1;
			blk_num = 0;
		}

		for(uint32_t k = 0; k < STORAGE_BLK_SIZ; k += 4)
		{
			_cur_addr = FL_START_ADDR + (blk_addr + blk_num) * STORAGE_BLK_SIZ + k;
			if(_cur_addr < FL_END_ADDR) _cur_word = flash_read(_cur_addr);

			for(int i = 0; i < 4; i++)
			{
				buf[blk_num * STORAGE_BLK_SIZ + k + i] = (uint8_t)(_cur_word & 0xFF);
				if(i != 3)_cur_word >>= 8;
			}
		}
	}

	return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */

	uint8_t _fl_erase = 1;

	uint32_t _cur_addr = 0;
	uint32_t _cur_word = 0;

	for(uint32_t blk_num = 0; blk_num < blk_len; blk_num++)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		GPIOA->CRH ^= GPIO_CRH_MODE9_0;
		GPIOA->BSRR = GPIO_BSRR_BS9;

		if(blk_num + blk_addr >= ((FL_END_ADDR_REAL - FL_START_ADDR) / FL_PAGE_SIZ))
		{
			blk_addr = ((FL_END_ADDR_REAL - FL_START_ADDR) / FL_PAGE_SIZ) - 1;
			blk_num = 0;
			_fl_erase = 0;
		}

		_cur_addr = FL_START_ADDR + (blk_addr + blk_num) * STORAGE_BLK_SIZ;
		if(_cur_addr < FL_END_ADDR && _fl_erase) flash_erase_page(_cur_addr);

		for(uint32_t k = 0; k < STORAGE_BLK_SIZ; k += 4)
		{
			_cur_word = 0;

			for(int8_t i = 3; i >= 0; i--)
			{
				_cur_word += buf[blk_num * STORAGE_BLK_SIZ + k + i];
				if(i != 0) _cur_word <<= 8;
			}

			flash_write(_cur_addr, _cur_word);

			_cur_addr += 4;
		}
	}

	return (USBD_OK);
  /* USER CODE END 7 */
}

/**
  * @brief  .
  * @param  None
  * @retval .
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
