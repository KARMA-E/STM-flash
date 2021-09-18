#ifndef _CONFIG_H_
#define _CONFIG_H_

#define	MULTI_CHIP_EN	(1)
#define SINGLE_CHIP_NUM	(3)
#define CHIP_QTY		(4)
#define	CHIP_MBIT_SIZE	(32)
#define USE_EXT_FLASH	(1u)
#define DEBUG_ENABLE	(0u)

#if MULTI_CHIP_EN
	#define CS_QTY		CHIP_QTY
#else
	#define CS_QTY		(1)
#endif

#endif
