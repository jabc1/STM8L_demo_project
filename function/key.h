#ifndef _key_h
#define _key_h
#include "stm8l15x_gpio.h"
#include "stm8l15x.h"

#define		MAX_SEND_NUM		9//max send num
#define		RE_SEND_NUM			2//resend num
#define		DATA_LEN			15//rf send data len

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
typedef struct{//垃圾，之前是怎么想的。flag、cmd、cmdtype能只留一个就好了
	u8 flag; 		  //区分学生还是老师
	u8 cmdtype;		  //设置命令类型 
	u8 cmd;			  //设置界面显示或者隐藏
	u8 id[5];	  	  //设备ID
	u8 data[6];		  //设置ABCDEF答案
	u8 crc;		      //校验位
}_INFO;
typedef struct{
	u8 resendnum;	//重发次数
	u8 ack;			//应答成功
	u8 noack;		//应答不成功
	u8 num;			//内容长度，即data[num]
	u8 restart;		//重启标志
	u8 resend;		//重发标志
	u8 key;			//按键是否有效
	u8 rfch;		//rf频道
	u8 rfflag;		//跳频标志
	u8 FREquency;	//单双频率切换,默认单频
}_COUNT;


extern _KEY		KeyFlag;
extern _INFO	InfoPack;
extern _COUNT	Run;

void key_gpio_inti(void);
void low_power_key_set(void);
void key_function(void);
void send_function(void);
void clear_xv(void);
u8 resend_function(void);
u8 check_ack(void);
u8 check_num(u8 num);
u8 check(void);
u8 check_data(u8 *dest,u8 data,u8 len);
u8 unpack(u8 *data);
u8 scanf_row1_key(void);
u8 scanf_row2_key(void);
u8 scanf_row3_key(void);
#endif







