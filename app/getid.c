/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : gitid.c
Description: 涓昏瀹炵幇鑾峰彇璁惧id鍔熻兘
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "getid.h"
#include "key.h"

void user_id(u8 *temp)
{
    temp[0] = *(__IO u8 *)(0X492D); 
    temp[1] = *(__IO u8 *)(0X492E); 
    temp[2] = *(__IO u8 *)(0X492F);
    temp[3] = *(__IO u8 *)(0X4930); 
    temp[4] = *(__IO u8 *)(0X4931); 
    /*
    temp[0] = 0x11;
    temp[1] = 0x22;
    temp[2] = 0x33;
    temp[3] = 0x44;
    temp[4] = 0x55;
    */
}

//用户ID读取，通过外部工具写入
void get_id(u32 Address,u8 Readlen)
{
	u8 i=0;
	FLASH_Unlock(FLASH_MemType_Program );
	while(FLASH_GetFlagStatus(FLASH_FLAG_PUL)==RESET)
	{
		;
	}
	for(i=0;i<Readlen;i++)
	{
	  InfoPack.id[i]=FLASH_ReadByte(Address+i);
	}
	FLASH_Lock(FLASH_MemType_Program );
	FLASH_DeInit();
}


u8  CheckSum(u8 *uBuff, u8 uBuffLen)
{
    u8 i,uSum=0;
    for(i=0; i<uBuffLen; i++)
    {
        uSum = uSum + uBuff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}







