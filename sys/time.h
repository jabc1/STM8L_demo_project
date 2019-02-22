#ifndef _time_h
#define _time_h
#include "stm8l15x.h"


typedef struct{
	u8 hour;
	u8 min;
	u8 sec;
	u16 ms;
}_TIME;
typedef struct{
	u8 lowcount;//进入低功耗开始计时
	u8 lowmode;//进入低功耗标志位
	u8 voltage;//检测电压标志
	u16 power;//自动休眠时间
	u16 sleep;//设置休眠时间
}_RUNCOUNT;


extern _TIME Time2;
extern _RUNCOUNT	RunTime;
void Init_time2(void);
void time2_del(void);

#endif









