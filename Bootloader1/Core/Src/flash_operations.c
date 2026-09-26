/*
 * flash_operations.c
 *
 *  Created on: 10 août 2026
 *      Author: anas
 */
#include "main.h"
#include "flash_operations.h"
#include "flash_layout.h"

void flash_read_sector(uint32_t sector_start_addr, uint32_t *buffer)
{
    uint32_t *flash_ptr = (uint32_t *)sector_start_addr;

    for (uint32_t i = 0; i < 5; i++)
    {
    	buffer[i] = flash_ptr[i];
    }
}

uint32_t flash_erase_app (void)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t error;

    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase.Sector       = APP_START_SECTOR;
    erase.NbSectors    = (APP_END_SECTOR - APP_START_SECTOR) + 1;

    HAL_FLASHEx_Erase(&erase, &error);
    return error;
}

uint32_t flash_erase_header (void)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t error;

    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase.Sector       = APP_HEADER_SECTOR;
    erase.NbSectors    = 1;

    HAL_FLASHEx_Erase(&erase, &error);
    return error;
}

void flash_write_word(uint32_t addr, uint32_t data)
{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
}
