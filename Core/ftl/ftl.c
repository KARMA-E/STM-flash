#include "ftl.h"


#define _BLOCK_BUF_SAVED	(-1)

uint8_t _cur_block_buf[W25Q_BLOCK_SIZ] = {0};
int32_t _cur_block_num = _BLOCK_BUF_SAVED;

typedef struct work_statistic_s
{
	uint8_t write_act;
	uint8_t read_act;
	uint8_t idle_cnt;
	uint32_t sectors_read_cnt;
	uint32_t sectors_write_cnt;
}work_statistic;

struct work_statistic_s _work_stat;


static void _load_block_buf(void)
{
	uint32_t die_block =	_cur_block_num / CS_QTY;
	uint32_t die_cs =		_cur_block_num % CS_QTY;

	W25Q80_set_cs_num(die_cs);
	uint32_t die_block_addr = W25Q_START_ADDR + (die_block * W25Q_BLOCK_SIZ);

	for(uint32_t page = 0; page < W25Q_BLOCK_SIZ; page += W25Q_PAGE_SIZ)
	{
		W25Q80_read_page(die_block_addr + page, (_cur_block_buf + page));
	}

	W25Q80_erase_block(die_block_addr);
}

static void _save_block_buf(void)
{
	uint32_t die_block =	_cur_block_num / CS_QTY;
	uint32_t die_cs =		_cur_block_num % CS_QTY;

	W25Q80_set_cs_num(die_cs);
	uint32_t die_block_addr = W25Q_START_ADDR + (die_block * W25Q_BLOCK_SIZ);

	for(uint32_t page = 0; page < W25Q_BLOCK_SIZ; page += W25Q_PAGE_SIZ)
	{
		W25Q80_write_page(die_block_addr + page, (_cur_block_buf + page));
	}
}

void FTL_init(void)
{
	_work_stat.idle_cnt = IDLE_WAIT_VAL;
	_work_stat.write_act = 0;
	_work_stat.read_act = 0;
	_work_stat.sectors_write_cnt = 0;
	_work_stat.sectors_read_cnt = 0;
}

void 		FTL_set_idle_cnt(uint8_t cnt) 	{ _work_stat.idle_cnt = cnt; }

uint8_t 	FTL_get_idle_cnt(void) 			{ return _work_stat.idle_cnt; }

uint32_t 	FTL_bytes_write_qty(void)		{ return _work_stat.sectors_write_cnt * STORAGE_BLK_SIZ; }

uint32_t 	FTL_bytes_read_qty(void)		{ return _work_stat.sectors_read_cnt * STORAGE_BLK_SIZ; }

void FTL_qty_reset(void)
{
	_work_stat.sectors_read_cnt = 0;
	_work_stat.sectors_write_cnt = 0;
}

uint8_t	FTL_check_write_act(uint8_t reset_act)
{
	uint8_t act = _work_stat.write_act;
	if(reset_act)
	{
		_work_stat.write_act = 0;
	}
	return act;
}

uint8_t	FTL_check_read_act(uint8_t reset_act)
{
	uint8_t act = _work_stat.read_act;
	if(reset_act)
	{
		_work_stat.read_act = 0;
	}
	return act;
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
	_work_stat.sectors_write_cnt++;
	_work_stat.write_act = 1;
	_work_stat.idle_cnt = IDLE_WAIT_VAL;

#if USE_EXT_FLASH

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
	_work_stat.sectors_read_cnt++;
	_work_stat.read_act = 1;
	_work_stat.idle_cnt = IDLE_WAIT_VAL;

#if USE_EXT_FLASH

	uint32_t block_num = (W25Q_START_ADDR + (sect_num * STORAGE_BLK_SIZ)) / W25Q_BLOCK_SIZ;

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

		for(uint32_t offset = 0; offset < STORAGE_BLK_SIZ; offset += W25Q_PAGE_SIZ)
		{
			//read_block_num = (start_addr + offset) / W25Q_BLOCK_SIZ;
			//uint32_t die_block =	read_block_num / CS_QTY;
			//uint32_t die_cs =		read_block_num % CS_QTY;
			//uint32_t page_addr = 	start_addr + offset % W25Q_BLOCK_SIZ;

			uint32_t die_block =	block_num / CS_QTY;
			uint32_t die_cs =		block_num % CS_QTY;
			uint32_t page_offset = 	(start_addr + offset) % W25Q_BLOCK_SIZ;

			W25Q80_set_cs_num(die_cs);
			W25Q80_read_page((die_block * W25Q_BLOCK_SIZ) + page_offset, (buf + offset));
		}
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




