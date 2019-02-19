#ifndef _getid_h
#define _getid_h
#include "stm8l15x.h"
#include "stm8l15x_flash.h"

#define		ID_ADD  		0xFF30

void user_id(u8 *temp);
void get_id(u32 Address,u8 Readlen);
u8  CheckSum(u8 *uBuff, u8 uBuffLen);

#endif




