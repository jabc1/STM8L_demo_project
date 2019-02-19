#ifndef _time_h
#define _time_h
#include "stm8l15x.h"

#pragma pack(push,1)
typedef struct{
	u8 hour;
	u8 min;
	u8 sec;
	u16 ms;
}_TIME;
typedef struct{
	u8 scount;//开机统计时间标志
	u8 lowmode;//低功耗标志位
	u8 voltage;//检测电压标志
	u16 power;//自动休眠时间
	u16 sleep;//休眠时间
}_RUNCOUNT;
#pragma pack(pop)

extern _TIME Time2;
extern _RUNCOUNT	RunTime;
void Init_time2(void);
void time2_del(void);

#endif









