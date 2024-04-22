#ifndef __stm32_flash_h
#define __stm32_flash_h

#include "stm32f4xx_hal.h"


#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define FLASH_SIZE  0x80000



#define FLASH_IS_EQU		0   /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
#define FLASH_REQ_WRITE		1	/* Flash不需要擦除，直接写 */
#define FLASH_REQ_ERASE		2	/* Flash需要先擦除,再写 */
#define FLASH_PARAM_ERR		3	/* 函数参数错误 */


//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//扇区11起始地址,128 Kbytes 
/*
#define ADDR_FLASH_SECTOR_12    ((uint32_t)0x08100000) 	//扇区12起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_13    ((uint32_t)0x08104000) 	//扇区13起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_14    ((uint32_t)0x08108000) 	//扇区14起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_15    ((uint32_t)0x0810C000) 	//扇区15起始地址,16 Kbytes 
#define ADDR_FLASH_SECTOR_16    ((uint32_t)0x08110000) 	//扇区16起始地址,64Kbytes 
#define ADDR_FLASH_SECTOR_17    ((uint32_t)0x08120000) 	//扇区17起始地址,128Kbytes 
#define ADDR_FLASH_SECTOR_18    ((uint32_t)0x08140000) 	//扇区18起始地址,128Kbytes 
#define ADDR_FLASH_SECTOR_19    ((uint32_t)0x08160000) 	//扇区19起始地址,128Kbytes
#define ADDR_FLASH_SECTOR_20    ((uint32_t)0x08180000) 	//扇区20起始地址,128Kbytes
#define ADDR_FLASH_SECTOR_21    ((uint32_t)0x081A0000) 	//扇区21起始地址,128Kbytes
#define ADDR_FLASH_SECTOR_22    ((uint32_t)0x081C0000) 	//扇区22起始地址,128Kbytes
#define ADDR_FLASH_SECTOR_23    ((uint32_t)0x081E0000) 	//扇区23起始地址,128Kbytes
*/


int32_t stm32_flashErase(uint32_t startAddr, uint32_t size );
int32_t stm32_flashWrite(uint32_t writeAddr, uint32_t* dataBuf, uint32_t size);
int32_t stm32_flashWrite_Dire(uint32_t writeAddr, uint32_t* dataBuf, uint32_t size);
//static uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
static uint32_t STMFLASH_ReadWord(uint32_t faddr);
void stm32_flashRead(uint32_t readAddr,uint32_t* dataBuf,uint16_t size);

#if 0
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize);
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize);
#endif

#endif
