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
#include "receivemode.h"

_KEY	KeyFlag;
_INFO	InfoPack;
_COUNT	Run;

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
	if(Run.key)
	{
		scanf_row1_key();
		scanf_row2_key();
		scanf_row3_key();
	}
}

void send_function()
{
	if( KeyFlag.head ||(KeyFlag.send &&(InfoPack.data[0] != NULL) ) )
	{
		show_sendfalg(TRUE);
		show_succ(FALSE);
		show_fail(FALSE);
	}
	if(KeyFlag.head)//head
	{
		RunTime.power = 0;
		KeyFlag.head = FALSE;
		memset(&InfoPack.data[0],0,6);
		get_id(ID_ADD,5);
		InfoPack.flag = 0x10;
		InfoPack.cmd = 'H';
		InfoPack.cmdtype = 0x00;//这三个变量好坑人啊，增加软件冗余
		InfoPack.crc = checksum((u8 *)(&InfoPack),DATA_LEN-1);
		unpack((u8 *)&InfoPack);
	}
	if(KeyFlag.send)//send
	{
		KeyFlag.send = FALSE;
		if(InfoPack.data[0] == NULL)
		{
			return;
		}
		if((KeyFlag.v)||(KeyFlag.x))
		{	
			InfoPack.cmdtype = 0xA1;//命令类型其实没有必要
		}
		else
		{
			InfoPack.cmdtype = 0xB2;
		}
		get_id(ID_ADD,5);
		InfoPack.flag = 0x10;//这个也可以不用
		InfoPack.cmd = 0x00;//这个也没有必要
		InfoPack.crc = checksum((u8 *)(&InfoPack),DATA_LEN-1);
		unpack((u8 *)&InfoPack);
	}
}
void clear_xv()
{
	KeyFlag.x = FALSE;
	KeyFlag.v = FALSE;
}
//带返回值主要是为了以后维护方便，至于变量说明详情见结构体定义说明
u8 resend_function()
{
	if(Run.resend == TRUE)
	{
		Run.resend = FALSE;
		unpack((u8 *)&InfoPack);
		if(Run.resendnum++ >= MAX_SEND_NUM)//send fail
		{
			Run.noack = TRUE;
			Run.resend = FALSE;//Send infomation fail
		}
	}
	if(Run.noack)//show fail flag
	{
		Sleep();
		Run.noack = FALSE;//clear send data. please restrat input
		Run.key = TRUE;
		Run.num = 0;
		Run.resendnum = 0;//重新统计
		memset(&InfoPack.data[0],0,6);
		memset(&KeyFlag,0,8);
		if((Run.ack >= 1)&&(Run.resendnum >= MAX_SEND_NUM))//收到一次数据时
		{
			show_succ(TRUE);
			show_sendfalg(FALSE);
			Run.ack = 0;
			return TRUE;
		}
		else
		{
			show_sendfalg(FALSE);
			show_fail(TRUE);
		}
	}
	return TRUE;
}
u8 unpack(u8 *data)
{
	Run.key = FALSE;
	Receive.init = FALSE;
	Receive.mode = FALSE;
	delay_us(1+InfoPack.id[4]);//显示发送标志
	//if(Run.FREquency)
	if(0)//使用单通道不进行调频
	{
		Run.rfflag = !Run.rfflag;
		(Run.rfflag == TRUE) ? (Run.rfch = 10) : (Run.rfch = 50);
	}
	else
	{
		Run.rfch = 10 ;
	}
	SPI24r1_Init();
	nrfsend(data,Run.rfch);
	check_ack();
	return TRUE;
}
u8 check_num(u8 num)
{
	if(++Run.ack >= num)//send succ
	{
		Run.ack = 0; 
	}
	else
	{
		Run.resend = TRUE;
		return FALSE;
	}
	return TRUE;
}
u8 check_ack()
{
	if(check())//显示ok标志，并取消发送标志，clear send data
	{
		//if(Run.FREquency)//跳频
		if(0)
		{
			if(!check_num(RE_SEND_NUM-1))
			{
				return FALSE;
			}
		}
		else
		{
			if(!check_num(RE_SEND_NUM))
			{
				return FALSE;
			}
		}
		Sleep();//nrf sleep mode
		show_succ(TRUE);
		show_sendfalg(FALSE);
		Run.key = TRUE;
		Run.num = 0;
		Run.resendnum = 0;//clear resendnum
		memset(&InfoPack.data[0],0,6);
		memset(&KeyFlag,0,8);
		return TRUE;
	}
	else//重新发送一次
	{
		Run.resend = TRUE;
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
u8 check_data(u8 *dest,u8 data,u8 len)
{
	u8 i;
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
	u8 key_status = 0;
	u8 long_key = 0,i = 0;
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
 		while(GPIO_ReadInputData(GPIOD) == 0xE0)
		{
			if(long_key++ >= 125)
			{
				Run.FREquency = ! Run.FREquency;
				key_status = 0xaa;
				break;
			}
			Delay_ms(2);
		}
		long_key = 0;
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
					InfoPack.data[Run.num++] = 'C';
				}
				break;
			case 0xd0://f
				KeyFlag.f1 = TRUE;
				if(check_data(&InfoPack.data[0],'F',6))
				{
					InfoPack.data[Run.num++] = 'F';
				}
				break;
			case 0xe0://x
				memset(&KeyFlag,0,8);
				memset(&InfoPack.data[0],0,6);
				Run.num = 0 ;
				InfoPack.data[Run.num] = 'X';
				KeyFlag.x = TRUE;
				break;
			case 0xaa:
				for(i=1;i<=6;i++)
				{
					if(Run.FREquency)
					{
						show_x(i&0x01);
					}
					else
					{
						show_v(i&0x01);
					}
					Delay_ms(80);
				}
				break;
		}
		if((key_status == 0xb0)||(key_status == 0xd0))
		{
			clear_xv();
		}
	}
	return TRUE;
}
u8 scanf_row2_key()
{
	u8 key_status=0;
	u8 long_key = 0;
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
 		while(GPIO_ReadInputData(GPIOD) == 0xE0)
		{
			if(long_key++ >= 125)
			{
				RunTime.lowmode = TRUE;
				key_status = 0;
				break;
			}
			Delay_ms(2);
		}
		long_key = 0;
		while((GPIO_ReadInputData(GPIOD))!=0xf0);//案件抬起
		switch(key_status)
		{
			case 0x70://null
				break;
			case 0xb0://b
				KeyFlag.b1 = TRUE;
				if(check_data(&InfoPack.data[0],'B',6))
				{
					InfoPack.data[Run.num++] = 'B';
				}
				break;
			case 0xd0://e
				KeyFlag.e1 = TRUE;
				if(check_data(&InfoPack.data[0],'E',6))
				{
					InfoPack.data[Run.num++] = 'E';
				}
				break;
			case 0xe0://clear
				memset(&KeyFlag,0,sizeof(KeyFlag));
				memset(&InfoPack.data[0],0,6);
				Run.num = 0;
				KeyFlag.clear = TRUE;			
				break;
		}
		if((key_status == 0xb0)||(key_status == 0xd0))
		{
			clear_xv();
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
					InfoPack.data[Run.num++] = 'A';
				}
				break;
			case 0xd0://d
				KeyFlag.d1 = TRUE;
				if(check_data(&InfoPack.data[0],'D',6))
				{
					InfoPack.data[Run.num++] = 'D';
				}
				break;
			case 0xe0://v
				memset(&KeyFlag,0,8);
				memset(&InfoPack.data[0],0,6);
				Run.num = 0 ;
				InfoPack.data[Run.num] = '/';
				KeyFlag.v = TRUE;
				break;
		}
		if((key_status == 0xb0)||(key_status == 0xd0))
		{
			clear_xv();
		}

	}
	return TRUE;
}










