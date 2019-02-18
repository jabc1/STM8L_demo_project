/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : show.c
Description: 主要实现lcd显示功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "show.h"
#include "lcd.h"
#include "key.h"
#include "string.h"

void show_function()
{
    show_x(KeyFlag.x);
    show_v(KeyFlag.v);
    show_a(KeyFlag.a1);
    show_b(KeyFlag.b1);
    show_c(KeyFlag.c1);
    show_d(KeyFlag.d1);
    show_e(KeyFlag.e1);
    show_f(KeyFlag.f1);
    
    if((KeyFlag.a1)||(KeyFlag.b1)||(KeyFlag.c1)||(KeyFlag.d1)||(KeyFlag.e1)||(KeyFlag.f1) \
    	||(KeyFlag.x)||(KeyFlag.v))
    {
    	show_succ(FALSE);
    	show_fail(FALSE);
    }
    if(KeyFlag.clear)
    {
    	KeyFlag.clear = FALSE;
    	show_succ(FALSE);
    	show_fail(FALSE);
    	memset(&InfoPack.data[0],0,6);
    }
}
void show_a(u8 flag)
{
    if(flag)
    {
        LCD->RAM[LCD_RAMRegister_1] |= (1<<2);
    }
    else
    {
        LCD->RAM[LCD_RAMRegister_1] &=~(1<<2);
    }
}
void show_b(u8 flag)
{
    if(flag)
    {
        LCD->RAM[LCD_RAMRegister_4] |= (1<<6);
    }
    else
    {
        LCD->RAM[LCD_RAMRegister_4] &=~(1<<6);
    }
}
void show_c(u8 flag)
{
    if(flag)
    {
      LCD->RAM[LCD_RAMRegister_8] |= (1<<2);
    }
    else
    {
      LCD->RAM[LCD_RAMRegister_8] &=~(1<<2);
    }
}
void show_d(u8 flag)
{
	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_11] |= (1<<6);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_11] &=~(1<<6);
	}
}
void show_e(u8 flag)
{
	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_1] |= (1<<3);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_1] &=~(1<<3);
	}
}
void show_f(u8 flag)
{
	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_4] |= (1<<7);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_4] &=~(1<<7);
	}
}
void show_v(u8 flag)
{
	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_12] |= (1<<1);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_12] &=~(1<<1);
	}
}
void show_x(u8 flag)
{
	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_8] |= (1<<5);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_8] &=~(1<<5);
	}
}

void show_succ(u8 flag)
{
  	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_8] |= (1<<6);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_8] &=~(1<<6);
	}
}

void show_fail(u8 flag)
{
  	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_12] |= (1<<2); 
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_12] &=~(1<<2); 
	}
}

void show_send(u8 flag)
{
  	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_5] |= (1<<2);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_5] &=~(1<<2);
	}
}
void show_signal(u8 flag)
{
  	if(flag)
	{
		LCD->RAM[LCD_RAMRegister_1] |= (1<<6);
	}
	else
	{
		LCD->RAM[LCD_RAMRegister_1] &=~(1<<6);
	}

}


















