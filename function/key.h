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
typedef struct{//������֮ǰ����ô��ġ�flag��cmd��cmdtype��ֻ��һ���ͺ���
	u8 flag; 		  //����ѧ��������ʦ
	u8 cmdtype;		  //������������ 
	u8 cmd;			  //���ý�����ʾ��������
	u8 id[5];	  	  //�豸ID
	u8 data[6];		  //����ABCDEF��
	u8 crc;		      //У��λ
}_INFO;
typedef struct{
	u8 resendnum;	//�ط�����
	u8 ack;			//Ӧ��ɹ�
	u8 noack;		//Ӧ�𲻳ɹ�
	u8 num;			//���ݳ��ȣ���data[num]
	u8 restart;		//������־
	u8 resend;		//�ط���־
	u8 key;			//�����Ƿ���Ч
	u8 rfch;		//rfƵ��
	u8 rfflag;		//��Ƶ��־
	u8 FREquency;	//��˫Ƶ���л�,Ĭ�ϵ�Ƶ
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







