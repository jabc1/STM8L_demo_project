/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : lowpower.c
Description: 主要实现设备进入低功耗状态功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-16
Modify	   : 
Modify Time: 
******************************************************************************/
#include "lowpower.h"
#include "sysconfig.h"
#include "getid.h"
#include "gpio.h"
#include "time.h"
#include "lcd.h"
#include "spi.h"
#include "nrf.h"
#include "Voltage.h"
#include "show.h"
#include "key.h"
#include "delay.h"
#include "stm8l15x_adc.h"

void lowpower_function()
{
	LCD_Clear();
	Adc_del();
	time2_del();
	LCD_DeInit();		
	CLK_PeripheralClockConfig (CLK_Peripheral_LCD,DISABLE);
	LCD_Cmd(DISABLE);
	SPI_DeInit(SPI1);	
	CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);
	SPI_Cmd(SPI1, DISABLE);  /* 使能SPI */
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC,DISABLE); 
	CLK_DeInit();
	CLK_HSICmd(DISABLE);
	CLK_LSICmd(DISABLE);
	low_power_key_set();
	CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);   
	PWR_FastWakeUpCmd(ENABLE);				 
	CLK_HSICmd(DISABLE);						  
	PWR_UltraLowPowerCmd(ENABLE);
	halt();
}
void restart_init()
{
	if(RunTime.lowmode)
	{
      	RunTime.lowmode= FALSE;
		lowpower_function();
	}  
	if(Count.restart)
	{
		disableInterrupts();
		CLK_Config();
		gpio_inti();
		lcd_init();
		Init_time2();
		gpio_inti();
		Check_voltage();
		show_signal(TRUE);
		key_gpio_inti();
		enableInterrupts();
		Count.restart = FALSE;
		RunTime.scount = TRUE;
		Time2.ms = 0;
		RunTime.power = 0;
	}
}
void low_power_key_set()
{
	GPIO_Init(GPIOA, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);//降低功耗
    GPIO_Init(GPIOB, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
//    GPIO_Init(GPIOD, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
//    GPIO_Init(GPIOE, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
//    GPIO_Init(GPIOF, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);
//    GPIO_Init(GPIOG, GPIO_Pin_All,  GPIO_Mode_Out_PP_High_Slow);

    GPIO_Init(GPIOD, GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);//1行
    GPIO_Init(GPIOD, GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);//2行
    GPIO_Init(GPIOD, GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);//3行
    GPIO_Init(GPIOD, GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);//4行 
    GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_In_PU_IT);//1列
    GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_In_PU_IT);//2列
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_IT);//3列
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_In_PU_IT);//4列
    


}
/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    EXTI_ClearITPendingBit(EXTI_IT_Pin0);
   	Count.restart = TRUE;
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
	EXTI_ClearITPendingBit(EXTI_IT_Pin1);
	Count.restart = TRUE;
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
	Count.restart = TRUE;
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
	Count.restart = TRUE;
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
















