/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : main.c
Description: 运行主循环
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-14
Modify	   : 
Modify Time: 
******************************************************************************/
#include "stm8l15x.h"
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
#include "lowpower.h"
#include "delay.h"

int main( void )
{
    disableInterrupts();//关闭系统总终端
    CLK_Config();
    gpio_inti();
    UsrID_Read(ID_ADD,6);
    lcd_init();
    Init_time2();
    Check_voltage();
    show_signal(TRUE);
    key_gpio_inti();
    RunTime.scount = TRUE;
    RunTime.power = 0;
    enableInterrupts();//开启系统总终端
    while(1)
    {
        key_function();
        show_function();
        send_function();
        restart_init();
        resend_function();
    }
}















