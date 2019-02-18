/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : Voltage.c
Description: ��Ҫʵ���豸������⼰��ʾ����
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
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//����ADC1ʱ��
    ADC_VrefintCmd(ENABLE); //ʹ���ڲ��ο���ѹ
    ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);//����ת����12λ��ת��ʱ��1��Ƶ
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);//ʹ���ڲ��ο���ѹͨ��
    ADC_Cmd(ADC1,ENABLE);//ADCʹ��
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
    float value=0.0;//����ʹ��
    
	Adc_Init();
	
    ADC_SoftwareStartConv(ADC1); //�������ת��
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//�����Ӧ��־
    adc1_value=ADC_GetConversionValue(ADC1); //��ȡת��ֵ

    value=(1.225*4096)/adc1_value; //���VDD��ѹ����λV
    
    LCD->RAM[LCD_RAMRegister_1] |= (1<<4); //��ؿ�
    
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











