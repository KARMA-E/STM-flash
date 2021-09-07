#include "ftl.h"


#define _WORK_STATE_VAL		(10)
#define _BLOCK_BUF_SAVED	(-1)

uint8_t _work_state_flag = 10;
uint8_t _cur_block_buf[W25Q_BLOCK_SIZ] = {0};
int32_t _cur_block_num = _BLOCK_BUF_SAVED;


uint32_t _sectors_read_cnt = 0;
uint32_t _sectors_write_cnt = 0;


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


void 		FTL_set_work_state(uint8_t state) 	{ _work_state_flag = state; }
uint8_t 	FTL_get_work_state(void) 			{ return _work_state_flag; }
uint32_t 	FTL_bytes_read_qty(void)			{ return _sectors_read_cnt * STORAGE_BLK_SIZ; }
uint32_t 	FTL_bytes_write_qty(void)			{ return _sectors_write_cnt * STORAGE_BLK_SIZ; }


void FTL_qty_reset(void)
{
	_sectors_read_cnt = 0;
	_sectors_write_cnt = 0;
}

void FTL_free_block_buf(void)
{
	if(_cur_block_num != _BLOCK_BUF_SAVED)
	{
		_save_block_buf();
		_cur_block_num = _BLOCK_BUF_SAVED;
	}
}

void FTL_storage_sector_write(uint32_t sect_num, uint8_t* buf)
{
	_sectors_write_cnt++;

	FTL_set_work_state(_WORK_STATE_VAL);

#if FTL_USE_EXT_FLASH

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

#else

	uint32_t cur_addr = 0;
	uint32_t cur_word = 0;

	cur_addr = FL_START_ADDR + (sect_num * FL_PAGE_SIZ);
	if(cur_addr < FL_END_ADDR) flash_erase_page(cur_addr);

	for(uint32_t k = 0; k < FL_PAGE_SIZ; k += 4)
	{
		cur_word = 0;

		for(int8_t i = 3; i >= 0; i--)
		{
			cur_word += buf[k + i];
			if(i != 0) cur_word <<= 8;
		}

		flash_write(cur_addr, cur_word);

		cur_addr += 4;
	}

#endif

}

void FTL_storage_sector_read(uint32_t sect_num, uint8_t* buf)
{
	_sectors_read_cnt++;
	_work_state_flag = _WORK_STATE_VAL;

#if FTL_USE_EXT_FLASH

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

#else

	uint32_t cur_addr = 0;
	uint32_t cur_word = 0;

	for(uint32_t k = 0; k < FL_PAGE_SIZ; k += 4)
	{
		cur_addr = FL_START_ADDR + (sect_num * FL_PAGE_SIZ) + k;
		if(cur_addr < FL_END_ADDR) cur_word = flash_read(cur_addr);

		for(int i = 0; i < 4; i++)
		{
			buf[k + i] = (uint8_t)(cur_word & 0xFF);
			if(i != 3)cur_word >>= 8;
		}
	}

#endif

}




