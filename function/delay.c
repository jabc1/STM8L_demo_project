/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : delay.c
Description: 主要实现软件延时功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "delay.h"

void delay_us(u16 num)//根本不准没有标准
{
    while(num--)
    {
        asm("NOP");
    }
}
void delay_200us(u8 num)//200us
{
	u8 i;
	while(num--)
	{
		for(i=43;i>0;i--)
		{
			asm("NOP");
		}
	}
}
void Delay_ms(u16 num)//接近1ms
{
	u8 i;
	while(num--)
	{
		for(i=200;i>0;i--)
		{
			asm("NOP");
			asm("NOP");
		}
	}
}















