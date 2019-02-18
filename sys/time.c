/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : time.c
Description: ��Ҫʵ�ֶ�ʱ����ʼ�����жϹ���
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-16
Modify	   : 
Modify Time: 
******************************************************************************/

#include "time.h"
#include "stm8l15x.h"
#include "stm8l15x_tim2.h"
#include "gpio.h"

_TIME Time2;
_RUNCOUNT	RunTime;


void Init_time2()
{
#if 0
    TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);    
    TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up, 0x7D);//1ms
    TIM2_ARRPreloadConfig(ENABLE);  
    TIM2_ITConfig(TIM2_IT_Update , ENABLE);  
    TIM2_Cmd(ENABLE);
#else
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);//����ʱ���ź��͸���ʱ��4(Lϵ�е�Ƭ������)
    TIM2_TimeBaseInit(TIM2_Prescaler_16,TIM2_CounterMode_Up,0x7D);//��ʱ��4,����ʱ��16��Ƶ.�Զ�װ��FA=125
    TIM2_SetCounter(0xFA);                                //�趨����ֵFA=125
    TIM2_ITConfig(TIM2_IT_Update,ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);                                    //������ʹ�ܣ���ʼ����  
#endif
}
void time2_del()
{
	TIM2_DeInit();
	CLK_PeripheralClockConfig (CLK_Peripheral_TIM2,DISABLE);
	TIM2_Cmd(DISABLE);
}
/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
	if (TIM2_GetITStatus(TIM2_IT_Update) != RESET)//
	{
		TIM2_ClearITPendingBit(TIM2_IT_Update);
		
		if(Time2.ms++ >= 124)
		{
			Time2.ms = 0;
			/*
			if(Time2.sec++ >= 59)
			{
				Time2.sec = 0;
				if(Time2.min++ >= 59)
				{
					Time2.min = 0;
				}
			}
			*/
			//GPIO_ToggleBits(GPIOF,GPIO_Pin_0);
			if(RunTime.scount)
			{
				if(RunTime.power++ >= 9)
				{
					RunTime.lowmode = TRUE;
				}
			}

		}
	}
}









