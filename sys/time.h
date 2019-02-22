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
	u8 lowcount;//����͹��Ŀ�ʼ��ʱ
	u8 lowmode;//����͹��ı�־λ
	u8 voltage;//����ѹ��־
	u16 power;//�Զ�����ʱ��
	u16 sleep;//��������ʱ��
}_RUNCOUNT;


extern _TIME Time2;
extern _RUNCOUNT	RunTime;
void Init_time2(void);
void time2_del(void);

#endif









