/*
 * OTA_implementation.c
 *
 *  Created on: 10 août 2026
 *      Author: anas
 */


#include <flash_layout.h>
#include "main.h"
#include "OTA_implementation.h"
#include "flash_operations.h"

#define OTA_FLAG_START		1
#define APP_HEADER_SECTOR	FLASH_SECTOR_2 //flash memory divided into sectors

uint32_t flash_buffer[5];

void enable_ota_request(void)
{
    HAL_FLASH_Unlock();

    flash_read_sector(APP_HEADER_ADDR, flash_buffer);//read

    /* Update only first word */
    flash_buffer[0] = OTA_FLAG_START;//modify

    flash_erase_sector(APP_HEADER_SECTOR);

    flash_write_sector(APP_HEADER_ADDR, flash_buffer);

    HAL_FLASH_Lock();

    NVIC_SystemReset();
}
