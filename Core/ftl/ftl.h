#ifndef _STORAGE_FTL_H
#define _STORAGE_FTL_H

#include "flash.h"
#include "w25q80.h"

#define FTL_USE_EXT_FLASH	(1u)
#define FTL_DEBUG_ENABLE	(0u)

#undef STORAGE_LUN_NBR
#undef STORAGE_BLK_NBR
#undef STORAGE_BLK_SIZ

#define STORAGE_LUN_NBR		(1u)

#if FTL_USE_EXT_FLASH
	#define STORAGE_BLK_SIZ		0x200
	#define STORAGE_BLK_NBR		(W25Q_MEM_SIZ / STORAGE_BLK_SIZ)
#else
	#define STORAGE_BLK_NBR		FL_PAGE_NUM
	#define STORAGE_BLK_SIZ		FL_PAGE_SIZ
#endif


#undef USB_HS_MAX_PACKET_SIZE
#define USB_HS_MAX_PACKET_SIZE 	STORAGE_BLK_SIZ

uint8_t 	FTL_get_work_state(void);
void 		FTL_set_work_state(uint8_t state);
uint32_t 	FTL_bytes_read_qty(void);
uint32_t 	FTL_bytes_write_qty(void);
void 		FTL_qty_reset(void);

void 		FTL_free_block_buf(void);
void 		FTL_storage_sector_write(uint32_t sect_num, uint8_t* buf);
void 		FTL_storage_sector_read(uint32_t sect_num, uint8_t* buf);

#endif


