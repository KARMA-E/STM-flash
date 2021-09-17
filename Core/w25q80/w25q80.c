#include "w25q80.h"
#include "hardware.h"


#define _W25Q_CMD_WREN			(0x06)
#define _W25Q_CMD_WRDIS			(0x04)
#define _W25Q_CMD_RD_STAT1		(0x05)
#define _W25Q_CMD_RD_STAT2		(0x35)
#define _W25Q_CMD_WR_STAT		(0x01)
#define _W25Q_CMD_PG_PROG		(0x02)
#define _W25Q_CMD_ERASE_SECT	(0x20)
#define _W25Q_CMD_ERASE_BLOCK	(0xD8)
#define _W25Q_CMD_RD_DATA		(0x03)
#define _W25Q_CMD_FAST_DATA		(0x0B)
#define _W25Q_CMD_READ_ID		(0x90)


SPI_HandleTypeDef* _p_hspi;
uint8_t _cs_cur = 0;

#pragma pack(push,1)

typedef struct _w25q80_cmd_addr_s
{
	uint8_t cmd;
	uint32_t addr;
} w25q80_cmd_addr_s;

#pragma pack(pop)


static void _spi_write(uint8_t* buf, uint16_t size)
{
	HAL_SPI_Transmit(_p_hspi, buf, size, 100);
}

static void _spi_read(uint8_t* buf, uint16_t size)
{
	HAL_SPI_Receive(_p_hspi, buf, size, 100);
}

static uint8_t _get_status(void)
{
	uint8_t status;
	uint8_t command = _W25Q_CMD_RD_STAT1;

	CS_reset(_cs_cur);
	_spi_write((uint8_t*)&command, 1);
	_spi_read((uint8_t*)&status, 1);
	CS_set();

	return status;
}

static inline void _write_enable(void)
{
	uint8_t cmd_buf = _W25Q_CMD_WREN;

	while(_get_status() & W25Q_STAT_BUSY) {;}

	CS_reset(_cs_cur);
	_spi_write((uint8_t*)&cmd_buf, 1);
	CS_set();
}


uint8_t W25Q80_init(SPI_HandleTypeDef* hspi)
{
	CS_set();
	_p_hspi = hspi;

	return 0;
}

uint16_t W25Q80_get_id(void)
{
	uint16_t id;
	uint8_t cmd_buf[4];

	cmd_buf[0] = _W25Q_CMD_READ_ID;
	cmd_buf[1] = 0;
	cmd_buf[2] = 0;
	cmd_buf[3] = 0;

	while(_get_status() & W25Q_STAT_BUSY) {;}

	CS_reset(_cs_cur);
	_spi_write(cmd_buf, 4);
	_spi_read((uint8_t*)(&id), 2);
	CS_set();

	return id;
}

void W25Q80_set_cs_num(uint8_t cs)
{
	_cs_cur = cs;
}

uint8_t W25Q80_erase_block(uint32_t addr)
{
	_write_enable();

	uint32_t tmp_addr = addr;
	uint8_t cmd_buf[4];

	cmd_buf[0] = _W25Q_CMD_ERASE_SECT;
	cmd_buf[3] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[2] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[1] = tmp_addr & 0xFF;

	while(_get_status() & W25Q_STAT_BUSY) {;}

	CS_reset(_cs_cur);
	_spi_write(cmd_buf, 4);
	CS_set();

	return 0;
}

uint8_t W25Q80_write_page(uint32_t addr, uint8_t* data_buf)
{
	_write_enable();

	uint32_t tmp_addr = addr;
	uint8_t cmd_buf[4];

	cmd_buf[0] = _W25Q_CMD_PG_PROG;
	cmd_buf[3] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[2] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[1] = tmp_addr & 0xFF;

	while(_get_status() & W25Q_STAT_BUSY) {;}

	CS_reset(_cs_cur);
	_spi_write(cmd_buf, 4);
	_spi_write(data_buf, 256);
	CS_set();

	return 0;
}

uint8_t W25Q80_read_page(uint32_t addr, uint8_t* data_buf)
{
	uint32_t tmp_addr = addr;
	uint8_t cmd_buf[5];

	cmd_buf[0] = _W25Q_CMD_FAST_DATA;
	cmd_buf[3] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[2] = tmp_addr & 0xFF;
	tmp_addr >>= 8;
	cmd_buf[1] = tmp_addr & 0xFF;
	cmd_buf[4] = 0;

	while(_get_status() & W25Q_STAT_BUSY) {;}

	CS_reset(_cs_cur);
	_spi_write(cmd_buf, 5);
	_spi_read(data_buf, 256);
	CS_set();

	return 0;
}


uint8_t W25Q80_erase_all(void)
{
	_write_enable();

	uint32_t tmp_addr = 0;
	uint8_t cmd_buf[4];

	for(int i = 0; i < W25Q_END_ADDR / W25Q_TARGET_SIZ; i++)
	{
		cmd_buf[0] = _W25Q_CMD_ERASE_BLOCK;
		cmd_buf[3] = tmp_addr & 0xFF;
		tmp_addr >>= 8;
		cmd_buf[2] = tmp_addr & 0xFF;
		tmp_addr >>= 8;
		cmd_buf[1] = tmp_addr & 0xFF;

		while(_get_status() & W25Q_STAT_BUSY) {;}

		CS_reset(_cs_cur);
		_spi_write(cmd_buf, 4);
		CS_set();

		tmp_addr += W25Q_TARGET_SIZ;
	}

	return 0;
}




