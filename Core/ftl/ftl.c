#include "ftl.h"

#define _WORK_STATE_VAL		(10)


uint8_t _work_state_flag = 10;
uint8_t _cur_block_buf[W25Q_BLOCK_SIZ] = {0};
int32_t _cur_block_num = -1;


static void _load_block_buf(void)
{
	uint32_t start_addr = W25Q_START_ADDR + (_cur_block_num * W25Q_BLOCK_SIZ);

	for(uint32_t page = 0; page < W25Q_BLOCK_SIZ; page += W25Q_PAGE_SIZ)
	{
		W25Q80_read_page(start_addr + page, (_cur_block_buf+ page));
	}
}

static void _save_block_buf(void)
{
	uint32_t start_addr = W25Q_START_ADDR + (_cur_block_num * W25Q_BLOCK_SIZ);

	W25Q80_erase_block(start_addr);

	for(uint32_t page = 0; page < W25Q_BLOCK_SIZ; page += W25Q_PAGE_SIZ)
	{
		W25Q80_write_page(start_addr + page, (_cur_block_buf + page));
	}
}


void 	FTL_set_work_state(uint8_t state) {_work_state_flag = state;}

uint8_t FTL_get_work_state(void) { return _work_state_flag;}


void FTL_free_block_buf(void)
{
	if(_cur_block_num != -1)
	{
		_save_block_buf();
		_cur_block_num = -1;
	}
}

void FTL_storage_sector_write(uint32_t sect_num, uint8_t* buf)
{
	FTL_set_work_state(_WORK_STATE_VAL);

	uint32_t block_num = (sect_num * STORAGE_BLK_SIZ) / W25Q_BLOCK_SIZ;

	if (block_num != _cur_block_num)
	{
		_save_block_buf();
		_cur_block_num = block_num;
		_load_block_buf();
	}

	uint32_t pos = (sect_num * STORAGE_BLK_SIZ) % W25Q_BLOCK_SIZ;

	for(uint32_t i=0; i<STORAGE_BLK_SIZ; i++)
	{
		_cur_block_buf[i + pos] = buf[i];
	}
}

void FTL_storage_sector_read(uint32_t sect_num, uint8_t* buf)
{
	FTL_set_work_state(_WORK_STATE_VAL);

	uint32_t block_num = (sect_num * STORAGE_BLK_SIZ) / W25Q_BLOCK_SIZ;

	if (block_num == _cur_block_num)
	{
		uint32_t pos = (sect_num * STORAGE_BLK_SIZ) % W25Q_BLOCK_SIZ;

		for(uint32_t i=0; i<STORAGE_BLK_SIZ; i++)
		{
			buf[i] = _cur_block_buf[i + pos];
		}
	}
	else
	{
		uint32_t start_addr = W25Q_START_ADDR + (sect_num * STORAGE_BLK_SIZ);

		W25Q80_read_page(start_addr, (buf));
		W25Q80_read_page(start_addr + W25Q_PAGE_SIZ, (buf + W25Q_PAGE_SIZ));
	}
}




