/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : lcd.c
Description: 主要实现lcd显示初始化功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/

#include "lcd.h"

void lcd_test1()
{
    LCD->RAM[LCD_RAMRegister_1] |= (1<<2);//A
}
void lcd_test2()
{
    LCD->RAM[LCD_RAMRegister_1] &=~ (1<<2);//A
}
void LCD_Clear()
{
    uint8_t counter=0;
    for(counter=0;counter<=LCD_RAMRegister_13;counter++)
    {
        LCD->RAM[counter]=LCD_RAM_RESET_VALUE;  
    }
}
void lcd_init()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
    LCD_Init(LCD_Prescaler_1, LCD_Divider_31, LCD_Duty_1_4, 
    				 LCD_Bias_1_3,   LCD_VoltageSource_External);
    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0x7F);
    //  LCD_ContrastConfig(LCD_Contrast_Level_0);
    LCD_ContrastConfig(LCD_Contrast_3V0);
    LCD_DeadTimeConfig(LCD_DeadTime_0);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);//LCD_PulseOnDuration_1
    LCD_Cmd(ENABLE);
    LCD_Clear();
}






