/*
 * flash_operations.h
 *
 *  Created on: 10 août 2026
 *      Author: anas
 */

#ifndef INC_FLASH_OPERATIONS_H_
#define INC_FLASH_OPERATIONS_H_

void flash_read_sector(uint32_t sector_start_addr, uint32_t *buffer);
uint32_t flash_erase_app (void);
void flash_write_word(uint32_t addr, uint32_t data);
uint32_t flash_erase_header (void);

#endif /* INC_FLASH_OPERATIONS_H_ */
