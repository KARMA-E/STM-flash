#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DEV_BOARD
//#define	LUT_BOARD

#ifdef DEV_BOARD
	#define CHIP_QTY		(1)
	#define	CHIP_MBIT_SIZE	(32)
#endif

#ifdef LUT_BOARD
	#define CHIP_QTY		(4)
	#define	CHIP_MBIT_SIZE	(32)
#endif

#define USE_EXT_FLASH		(1u)
#define DEBUG_ENABLE		(0u)

#endif
