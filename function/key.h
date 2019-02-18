#ifndef _key_h
#define _key_h
#include "stm8l15x_gpio.h"
#include "stm8l15x.h"
#pragma pack(push,1)
typedef struct{
	u8 a1;
	u8 b1;
	u8 c1;
	u8 d1;
	u8 e1;
	u8 f1;
	u8 x;
	u8 v;
	u8 head;
	u8 clear;
	u8 send;
}_KEY;
typedef struct{
	u8 id[5];
	u8 data[6];
	u8 crc;
	u8 temp[3];
}_INFO;
typedef struct{
	u8 sendnum;//发送次数
	u8 ack;//应答成功
	u8 noack;//应答不成功
	u8 num;//内容长度
	u8 restart;//重启标志
	u8 resend;//重发标志
}_COUNT;
#pragma pack(pop)

extern _KEY		KeyFlag;
extern _INFO	InfoPack;
extern _COUNT	Count;

void key_gpio_inti(void);
void low_power_key_set(void);
void key_function(void);
void send_function(void);

u8 resend_function(void);
u8 check_ack(void);
u8 check(void);
u8 check_data(u8 *dest,u8 data,u8 len);
u8 pack(u8 *data,u8 ch);
u8 clear_xv(u8 temp);
u8 scanf_row1_key(void);
u8 scanf_row2_key(void);
u8 scanf_row3_key(void);


#endif







