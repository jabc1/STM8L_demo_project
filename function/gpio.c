/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : gpio.c
Description: 主要实现gpio初始化
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "gpio.h"
#include "delay.h"
void SI24r1_Gpioinit()
{
	GPIO_Init(CE_PORT, CE_PORT_PIN, GPIO_Mode_Out_PP_High_Fast);//ce 
	GPIO_Init(CSN_PORT,CSN_PORT_PIN, GPIO_Mode_Out_PP_High_Fast);//cs
	GPIO_Init(SCK_PORT,SCK_PORT_PIN , GPIO_Mode_Out_PP_High_Fast);//sck
	GPIO_Init(MOSI_PORT,MOSI_PORT_PIN, GPIO_Mode_Out_PP_High_Fast);//MOSI
	GPIO_Init(MISO_PORT,MISO_PORT_PIN, GPIO_Mode_In_FL_No_IT);//MISO
	GPIO_Init(IRQ_PORT, IQR_PORT_PIN, GPIO_Mode_In_FL_No_IT); //IRQ
}

void gpio_inti()
{
	GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast);//ce 
}













