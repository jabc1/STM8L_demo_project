#include "sysconfig.h"
#include "delay.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_pwr.h"
void CLK_Config(void)
{   
	CLK_SYSCLKSourceSwitchCmd(ENABLE); 
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI); 
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_64);
	
	while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)//16m 
	{	} 
	CLK_HSICmd(ENABLE);
	CLK_HSEConfig(CLK_HSE_OFF);
	CLK_LSEConfig(CLK_HSE_OFF);
	PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能
	PWR_UltraLowPowerCmd(ENABLE);
}








