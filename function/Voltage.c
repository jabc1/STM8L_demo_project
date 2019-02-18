/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : Voltage.c
Description: 主要实现设备电量检测及显示功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-16
Modify	   : 
Modify Time: 
******************************************************************************/
#include "Voltage.h"
#include "stm8l15x_adc.h"
#include "lcd.h"

void Adc_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//开启ADC1时钟
    ADC_VrefintCmd(ENABLE); //使能内部参考电压
    ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);//连续转换，12位，转换时钟1分频
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);//使能内部参考电压通道
    ADC_Cmd(ADC1,ENABLE);//ADC使能
}
void Adc_del()
{
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,DISABLE);
	ADC_DeInit(ADC1);
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	ADC_Cmd(ADC1,DISABLE);
}
void Check_voltage()
{
    u16 adc1_value=0;
    float value=0.0;//测试使用
    
	Adc_Init();
	
    ADC_SoftwareStartConv(ADC1); //开启软件转换
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
    adc1_value=ADC_GetConversionValue(ADC1); //获取转换值

    value=(1.225*4096)/adc1_value; //获得VDD电压，单位V
    
    LCD->RAM[LCD_RAMRegister_1] |= (1<<4); //电池框
    
    if(value >=3.0)
    {
        LCD->RAM[LCD_RAMRegister_5] |= (1<<0);
        LCD->RAM[LCD_RAMRegister_8] |= (1<<4);	
        LCD->RAM[LCD_RAMRegister_12] |= (1<< 0);
    }
    else if((value >=2.8) && (value <3.0))
    {
        LCD->RAM[LCD_RAMRegister_5] &= ~(1<<0);
        LCD->RAM[LCD_RAMRegister_8] |= (1<<4);	
        LCD->RAM[LCD_RAMRegister_12] |= (1<< 0);

    }
    else if((value >=2.5) && (value <2.8) )
    {
        LCD->RAM[LCD_RAMRegister_5] &= ~(1<<0);
        LCD->RAM[LCD_RAMRegister_8] &= ~ (1<<4);	
        LCD->RAM[LCD_RAMRegister_12] |= (1<< 0);

    }
    else//if(value <2.5)
    { 
        LCD->RAM[LCD_RAMRegister_5] &= ~ (1<<0);
        LCD->RAM[LCD_RAMRegister_8] &= ~ (1<<4);	
        LCD->RAM[LCD_RAMRegister_12] &= ~ (1<< 0);
    }
	Adc_del();

}











