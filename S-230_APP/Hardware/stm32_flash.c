#include "stm32_flash.h"

#define SECTOR_MAX_NUMBER 7u


static FLASH_EraseInitTypeDef EraseInitStruct;

/**
* 函数功能: 根据输入的地址给出它所在的sector
* 输入参数: Address flash地址
* 返 回 值: 无
* 说    明: 无
*/
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;  
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_SECTOR_9;  
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;  
	}
	else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
	{
		sector = FLASH_SECTOR_11;  
	}

	return sector;
}

/**
* 函数功能: 根据扇区编号获取扇区大小
* 输入参数: Sector 
* 返 回 值: 无
* 说    明: 无
*/
static uint32_t GetSectorSize(uint32_t Sector)
{
	uint32_t sectorsize = 0x00;

	if((FLASH_SECTOR_0 == Sector) || (FLASH_SECTOR_1 == Sector) || (FLASH_SECTOR_2 == Sector) || (FLASH_SECTOR_3 == Sector))
	{
		sectorsize = 16 * 1024;
	}
	else if(FLASH_SECTOR_4 == Sector)
	{
		sectorsize = 64 * 1024;
	}
	else
	{
		sectorsize = 128 * 1024;
	}
	
	return sectorsize;
}


/***************************************
//函数名称：STMFLASH_Erase
//函数功能：从指定的扇区首地址开始，擦除指定大小的FLASH空间
//函数参数：startAddr :需要擦除的flash空间的首地址（必须为扇区的首地址）
            size: 需要擦除的flash空间大小（单位字节）
//函数返回：0 成功 -1 非法地址 -2 指定擦除区域超出flash的空间大小  -3擦除失败
***************************************/
int32_t stm32_flashErase(uint32_t startAddr,uint32_t size )
{
	uint32_t sectorSize = 0;
	uint32_t startSector = 0;
	uint32_t sectorIndex = 0;
	uint32_t err = 0;
	uint32_t end_addr = startAddr + size;
	
	HAL_FLASH_Unlock();
	/* 获取要擦除的首个扇区 */
	startSector = GetSector(startAddr);
	sectorIndex = startSector;
	while(size)
	{
		sectorSize = GetSectorSize(sectorIndex);
		sectorIndex += 1;
		if(size < sectorSize)
		{
			break;
		}
		size -= sectorSize;
		startAddr += sectorSize;
	}
	/* 初始化擦除结构体 */
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = startSector;
	EraseInitStruct.NbSectors = (sectorIndex - startSector);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	if(HAL_OK != HAL_FLASHEx_Erase(&EraseInitStruct, &err))
	{
        HAL_FLASH_Lock();
		return -1;
	}
	HAL_FLASH_Lock();
	
	return 0;
}
/*******************************************
//函数名称：stm32_flashWrite
//函数功能：写数据到flash
//函数参数：writeAddr 目标首地址 dataBuf 数据缓冲区指针 size 数据大小(单位 字节)
//函数返回：0 写入成功 -1 写入异常 -2 擦除扇区失败
*******************************************/
int32_t stm32_flashWrite(uint32_t writeAddr, uint32_t* dataBuf, uint32_t size)
{
    uint32_t endAddr = writeAddr + size;
	
    HAL_FLASH_Unlock();	//解锁 
    if (stm32_flashErase(writeAddr,size) == 0)
    {
		HAL_FLASH_Unlock();	//解锁 
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);	
		while(writeAddr < endAddr)//写数据
		{	
			if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAddr, *dataBuf))//写入数据
			{ 
            	HAL_FLASH_Lock();
				return -1;//写入异常
			}
			writeAddr += 4;//每次写入四字节，所以地址加4
			dataBuf++;
		} 
    }
    else 
	{
		return -2;//擦除扇区失败
	}
	HAL_FLASH_Lock();//上锁
	
	return 0;//写入成功
}


/*******************************************
//函数名称：stm32_flashWrite
//函数功能：写数据到flash
//函数参数：writeAddr 目标首地址 dataBuf 数据缓冲区指针 size 数据大小(单位 字节)
//函数返回：0 写入成功 -1 写入异常 -2 擦除扇区失败
*******************************************/
int32_t stm32_flashWrite_Dire(uint32_t writeAddr,uint32_t* dataBuf,uint32_t size)
{
	uint32_t endAddr = writeAddr + size;
	
    HAL_FLASH_Unlock();	//解锁 
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	while(writeAddr < endAddr)//写数据
	{	
		if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAddr, *dataBuf))//写入数据
		{ 
            HAL_FLASH_Lock();
			return -1;//写入异常
		}
		writeAddr += 4;//每次写入四字节，所以地址加4
		dataBuf++;
	} 
	HAL_FLASH_Lock();//上锁
	
	return 0;//写入成功
}



//读取指定地址的半字(16位数据)
//faddr:读地址
//返回值:对应数据.
//static uint16_t STMFLASH_ReadHalfWord(u32 faddr)
//{
//    return *(vu16*)faddr;
//}

//读取指定地址的字(32位数据)
//faddr:读地址
//返回值:对应数据.
static uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(__IO uint32_t *)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void stm32_flashRead(uint32_t readAddr,uint32_t* dataBuf,uint16_t size)
{
    uint16_t i = 0;
    for(i=0; i<size; i++)
	{
		dataBuf[i] = STMFLASH_ReadWord(readAddr);//读取4个字节.
		readAddr += 4;//偏移4个字节.	
	}
}



/*
*********************************************************************************************************
*	函 数 名: bsp_ReadCpuFlash
*	功能说明: 读取CPU Flash的内容
*	形    参：_ucpDst : 目标缓冲区
*			 _ulFlashAddr : 起始地址
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0=成功，1=失败
*********************************************************************************************************
*/
#if 0
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > STM32_FLASH_BASE + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
	if (_ulSize == 0)
	{
		return 1;
	}

	for (i=0; i<_ulSize; i++)
	{
		*_ucpDst++ = *(uint8_t *)_ulFlashAddr++;
	}

	return 0;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: bsp_CmpCpuFlash
*	功能说明: 比较Flash指定地址的数据.
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpBuf : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值:
*			FLASH_IS_EQU		0   Flash内容和待写入的数据相等，不需要擦除和写操作
*			FLASH_REQ_WRITE		1	Flash不需要擦除，直接写
*			FLASH_REQ_ERASE		2	Flash需要先擦除,再写
*			FLASH_PARAM_ERR		3	函数参数错误
*********************************************************************************************************
*/
#if 0
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* 相等标志 */
	uint8_t ucByte;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > STM32_FLASH_BASE + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*　函数参数错误　*/
	}

	/* 长度为0时返回正确 */
	if (0 == _ulSize)
	{
		return FLASH_IS_EQU;		/* Flash内容和待写入的数据相等 */
	}

	ucIsEqu = 1;			/* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	for (i=0; i<_ulSize; i++)
	{
		ucByte = *(uint8_t *)_ulFlashAddr;

		if (ucByte != *_ucpBuf)
		{
			if (0xFF != ucByte)
			{
				return FLASH_REQ_ERASE;		/* 需要擦除后再写 */
			}
			else
			{
				ucIsEqu = 0;	/* 不相等，需要写 */
			}
		}
		_ulFlashAddr++;
		_ucpBuf++;
	}

	if (1 == ucIsEqu)
	{
		return FLASH_IS_EQU;	/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash不需要擦除，直接写 */
	}
}
#endif


/*
*********************************************************************************************************
*	函 数 名: bsp_WriteCpuFlash
*	功能说明: 写数据到CPU 内部Flash。
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpSrc : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0-成功，1-数据长度或地址溢出，2-写Flash出错(估计Flash寿命到)
*********************************************************************************************************
*/
#if 0
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucRet;
	uint16_t usTemp;
	FLASH_Status status = FLASH_COMPLETE;

  RCC_HSICmd(ENABLE);

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > STM32_FLASH_BASE + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0 时不继续操作  */
	if (0 == _ulSize)
	{
		return 0;
	}

	/* 长度为奇数时不继续操作  */
	if (0!= (_ulSize % 2))
	{
		return 1;
	}	

	ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);

	if (FLASH_IS_EQU == ucRet)
	{
		return 0;
	}

	__set_PRIMASK(1);  		/* 关中断 */

	/* FLASH 解锁 */
	FLASH_Unlock();

  	/* Clear pending flags (if any) */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);	

	/* 需要擦除 */
	if (ucRet == FLASH_REQ_ERASE)
	{
//		status = FLASH_ErasePage(bsp_GetSector(_ulFlashAddr));
		status = FLASH_EraseSector(stm32_flashGetFlashSector(_ulFlashAddr),VoltageRange_3);//VCC=2.7~3.6V之间!!
		if (status != FLASH_COMPLETE)
		{
			return 2;
		}		
	}

	/* 按字节模式编程（为提高效率，可以按字编程，一次写入4字节） */
	for (i = 0; i < _ulSize / 2; i++)
	{
		//FLASH_ProgramByte(_ulFlashAddr++, *_ucpSrc++);		
		usTemp = _ucpSrc[2 * i];
		usTemp |= (_ucpSrc[2 * i + 1] << 8);
		status = FLASH_ProgramHalfWord(_ulFlashAddr, usTemp);
		if (status != FLASH_COMPLETE)
		{
			break;
		}
		
		_ulFlashAddr += 2;
	}

  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	FLASH_Lock();

  	__set_PRIMASK(0);  		/* 开中断 */

	if (status == FLASH_COMPLETE)
	{
		return 0;
	}
	return 2;
}
#endif


int InternalFlash_Test(void)
{
	
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   (ADDR_FLASH_SECTOR_5)   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     (FLASH_USER_START_ADDR + GetSectorSize(GetSector(FLASH_USER_START_ADDR)) -1)
/*准备写入的测试数据*/
#define DATA_32                 ((uint32_t)0x87654321)
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;


  /* 解锁,删除和写入必须先解锁 */
  HAL_FLASH_Unlock();
  
  /* 获取要擦除的首个扇区 */
  FirstSector = GetSector(FLASH_USER_START_ADDR);
  
  /* 获取要擦除的扇区数目 */
  NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1; 
  
  /* 初始化擦除结构体 */
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /* 此处可以使用HAL_FLASH_GetError()来获取扇区擦除代码错误 */
    while (1)
    {
    	MemoryProgramStatus = HAL_FLASH_GetError();
    }
  }
  Address = FLASH_USER_START_ADDR;
  /* 按字word写入数据到内部flash */
  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
    {
      Address = Address + 4;
    }
    else
    { 
      while (1)
      {
      }
    }
  }
  /* 锁定，如果是读取的话，无需解锁 */
  HAL_FLASH_Lock(); 
  
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0;
  /* 读取数据验证是否和写入的数据相同，如果相同，那么MemoryProgramStatus=0 */
  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t*)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;  
    }

    Address = Address + 4;
  }  
  if (MemoryProgramStatus )
  {
    return -1;
  }
  else
  {
   return 0;
  }
  
}

