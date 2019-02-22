#ifndef _receivemode_h
#define _receivemode_h

#include "stm8l15x.h"
#include "nrf.h"
#include "gpio.h"
#include "stm8l15x_spi.h"
typedef struct{
	u8 flag;
	u8 mode;
	u8 init;
	u16 restat;//重启
	u16 count;//重启计时器
}_RECEIVE;
extern _RECEIVE Receive;
void receive_init(void);
u8 nrf_receive_mode(u8 ch,u8 len);
u8 receive_function(void);
u8 receive_data(u8 cmd,u8 datalen,u8 *data);
u8 spi_write(u8 cmd);
u8 read_status(u8 cmd);







#endif









