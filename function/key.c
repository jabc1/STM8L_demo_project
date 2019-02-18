/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : key.c
Description: 主要实现按键功能及按键业务逻辑功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "key.h"
#include "delay.h"
#include "getid.h"
#include "nrf.h"
#include "string.h"
#include "time.h"
#include "lowpower.h"
#include "show.h"

_KEY	KeyFlag;
_INFO	InfoPack;
_COUNT	Count;

void key_gpio_inti()
{

	GPIO_Init(GPIOA, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);//主要是为了降低功耗
    GPIO_Init(GPIOB, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOE, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);

	GPIO_Init(GPIOC, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast);//1列
	GPIO_Init(GPIOC, GPIO_Pin_1,GPIO_Mode_Out_PP_High_Fast);//2列
	GPIO_Init(GPIOC, GPIO_Pin_2,GPIO_Mode_Out_PP_High_Fast);//3列
	GPIO_Init(GPIOC, GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
	
	GPIO_Init(GPIOD, GPIO_Pin_4,GPIO_Mode_In_PU_No_IT);//1行
	GPIO_Init(GPIOD, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);//2行
	GPIO_Init(GPIOD, GPIO_Pin_6,GPIO_Mode_In_PU_No_IT);//3行
	GPIO_Init(GPIOD, GPIO_Pin_5,GPIO_Mode_In_PU_No_IT);//4行
}

void key_function()
{
	scanf_row1_key();
	scanf_row2_key();
	scanf_row3_key();
}

void send_function()
{
	u8 temp,i,j=0;
	if((KeyFlag.head)||((KeyFlag.send)&&(InfoPack.data[0] != NULL)))
	{
		show_send(TRUE);
		show_succ(FALSE);
		show_fail(FALSE);
		Delay_ms(100+InfoPack.id[4]);//显示发送标志
	}
	if(KeyFlag.head)//head
	{
		KeyFlag.head = FALSE;
		memset(&InfoPack.data[0],0,6);
		InfoPack.data[0] = 'H';
		InfoPack.crc = CheckSum((u8 *)(&InfoPack),11);
		InfoPack.temp[0] = 0xaa;
		InfoPack.temp[1] = 0xbb;
		InfoPack.temp[2] = 0xcc;
		pack((u8 *)&InfoPack,10);
	}
	if(KeyFlag.send)//send
	{
		KeyFlag.send = FALSE;
		if(InfoPack.data[0] == NULL)
		{
			return;
		}
		InfoPack.crc = CheckSum((u8 *)(&InfoPack),11);
		pack((u8 *)&InfoPack,10);
	}
}

//带返回值主要是为了以后维护方便
u8 resend_function()
{
	if(Count.resend == TRUE)
	{
		Count.resend = FALSE;
		pack((u8 *)&InfoPack,10);
		if(Count.sendnum++ >= 3)//send fail
		{
			Count.sendnum = 0;
			Count.resend = FALSE;
			Count.noack = TRUE;
		}
	}
	if(Count.noack)//show fail flag
	{
		Count.noack = FALSE;//clear send data please restrat input
		Count.num = 0;
		memset((u8 *)&InfoPack,0,sizeof(InfoPack));
		memset(&KeyFlag,0,8);
		show_send(FALSE);
		show_fail(TRUE);
		Sleep();
	}
	return TRUE;
}
u8 pack(u8 *data,u8 ch)
{
	SPI24r1_Init();
	nrfsend(data,ch);
	check_ack();
	return TRUE;
}
u8 check_ack()
{
	if(check())//显示ok标志，并取消发送标志，clear send data
	{
		Sleep();//nrf sleep mode
		show_succ(TRUE);
		show_send(FALSE);
		Count.num = 0;
		memset((u8 *)&InfoPack,0,sizeof(InfoPack));
		memset(&KeyFlag,0,8);		

		lowpower_function();
	}
	else//重新发送一次
	{
		Count.resend = TRUE;
	}
	return TRUE;
}
u8 check()
{
	u8 state=0;
	state = SPI_Read_Reg(SPI1,STATUS);
	if(state == 0x2E)
	{
		SPI_RW_Reg(WRITE_REG+STATUS,SPI1,0x7E);
		return TRUE;
	}
	return FALSE;
}

u8 clear_xv(u8 temp)
{
	KeyFlag.x = FALSE;
	KeyFlag.v = FALSE;
	return TRUE;
}
u8 check_data(u8 *dest,u8 data,u8 len)
{
	u8 i=0;
	for(i=0;i<len;i++)
	{
		if(dest[i] == data)
		{
			return FALSE;
		}  
	}
	return TRUE;
}
//按键消抖时间差不多是8ms
u8 scanf_row1_key()
{
	u8 key_status=0;
	//第一行设置为低，然后读取列中是否有低的
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	key_status = GPIO_ReadInputData(GPIOD);
	if(key_status !=0xf0)
	{
		delay_200us(40);//使用软件软件延时进行小豆，赶时间，要使用复杂小豆自行添加
		key_status = GPIO_ReadInputData(GPIOD);
		while((GPIO_ReadInputData(GPIOD))!=0xf0);//案件抬起
		switch(key_status)
		{
			case 0x70://send
				KeyFlag.send = TRUE;
				break;
			case 0xb0://c
				KeyFlag.c1 = TRUE;
				if(check_data(&InfoPack.data[0],'C',6))
				{
					InfoPack.data[Count.num++] = 'C';
				}
				clear_xv(1);
				break;
			case 0xd0://f
				KeyFlag.f1 = TRUE;
				if(check_data(&InfoPack.data[0],'F',6))
				{
					InfoPack.data[Count.num++] = 'F';
				}
				clear_xv(1);
				break;
			case 0xe0://x
				memset(&KeyFlag,0,8);
				memset(&InfoPack.data[0],0,6);
				Count.num = 0 ;
				InfoPack.data[Count.num] = 'X';
				KeyFlag.x = TRUE;
				break;
		}
	}
	return TRUE;
}
u8 scanf_row2_key()
{
	u8 key_status=0;
	//第二行设置为低，然后读取列中是否有低的
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	key_status = GPIO_ReadInputData(GPIOD);
	if(key_status !=0xf0)
	{
		delay_200us(40);//使用软件软件延时进行小豆，赶时间，要使用复杂小豆自行添加
		key_status = GPIO_ReadInputData(GPIOD);
		while((GPIO_ReadInputData(GPIOD))!=0xf0);//案件抬起
		switch(key_status)
		{
			case 0x70://null
				break;
			case 0xb0://b
				KeyFlag.b1 = TRUE;
				if(check_data(&InfoPack.data[0],'B',6))
				{
					InfoPack.data[Count.num++] = 'B';
				}
				clear_xv(1);
				break;
			case 0xd0://e
				KeyFlag.e1 = TRUE;
				if(check_data(&InfoPack.data[0],'E',6))
				{
					InfoPack.data[Count.num++] = 'E';
				}
				clear_xv(1);
				break;
			case 0xe0://clear
				memset(&KeyFlag,0,sizeof(KeyFlag));
				KeyFlag.clear = TRUE;			
				break;
		}
	}
	return TRUE;
}
u8 scanf_row3_key()
{
	u8 key_status=0;
	//第三行设置为低，然后读取列中是否有低的
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	key_status = GPIO_ReadInputData(GPIOD);
	if(key_status !=0xf0)
	{
		delay_200us(40);//使用软件软件延时进行小豆，赶时间，要使用复杂小豆自行添加
		key_status = GPIO_ReadInputData(GPIOD);
		while((GPIO_ReadInputData(GPIOD))!=0xf0);//案件抬起
		switch(key_status)
		{
			case 0x70://head
				memset(&KeyFlag,0,8);
				KeyFlag.head = TRUE;			
				break;
			case 0xb0://a
				KeyFlag.a1 = TRUE;
				if(check_data(&InfoPack.data[0],'A',6))
				{
					InfoPack.data[Count.num++] = 'A';
				}
				clear_xv(1);
				break;
			case 0xd0://d
				KeyFlag.d1 = TRUE;
				if(check_data(&InfoPack.data[0],'D',6))
				{
					InfoPack.data[Count.num++] = 'D';
				}
				clear_xv(1);
				break;
			case 0xe0://v
				memset(&KeyFlag,0,8);
				memset(&InfoPack.data[0],0,6);
				Count.num = 0 ;
				InfoPack.data[Count.num] = '/';
				KeyFlag.v = TRUE;
				break;
		}
	}
	return TRUE;
}










