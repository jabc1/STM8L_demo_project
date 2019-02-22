#include "receivemode.h"
#include "show.h"
#include "delay.h"
#include "lcd.h"
#include "key.h"
#include "string.h"
_RECEIVE Receive;
u8 redata[15] = {0,};
u8 nrf_receive_mode(u8 ch,u8 len)
{
	SPI24r1_Init();
	GPIO_L(CE_PORT);
	SPI_Write_Buf(WRITE_REG + TX_ADDR,SPI1,TX_ADDRESS, TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,SPI1,TX_ADDRESS, TX_ADR_WIDTH);
	SPI_RW_Reg(WRITE_REG + EN_AA ,SPI1,0x01);//Compatible Nodic:0x00
	SPI_RW_Reg(WRITE_REG + EN_RXADDR ,SPI1,0x0F);//Reserved
	SPI_RW_Reg(WRITE_REG + SETUP_AW,SPI1,0x01);//Send Address width:3u8
	SPI_RW_Reg(WRITE_REG + RF_CH,SPI1,ch);
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,SPI1,len);
	SPI_RW_Reg(WRITE_REG + RF_SETUP,SPI1,0x0f);
	spi_write(FLUSH_RX);
	SPI_RW_Reg(WRITE_REG + STATUS,SPI1,0xFF);
	SPI_RW_Reg(WRITE_REG + CONFIG,SPI1,0x0B);
	GPIO_H(CE_PORT);
	return TRUE;
}
u8 spi_write(u8 cmd)
{
	GPIO_L(CSN_PORT);
	SPI_SendByte(SPI1,cmd);
	GPIO_H(CSN_PORT);
	return TRUE;
}
u8 read_status(u8 cmd)
{
	u8 temp;
	GPIO_L(CSN_PORT);
	SPI_SendByte(SPI1,cmd);
	temp = SPI_SendByte(SPI1,0x00);
	GPIO_H(CSN_PORT);
	return temp;
}
u8 receive_data(u8 cmd,u8 datalen,u8 *data)
{
	u8 i = 0,status;
	GPIO_L(CSN_PORT);
	status = SPI_SendByte(SPI1,cmd);
	for(i=0;i<datalen;i++)
	{
		data[i] = SPI_SendByte(SPI1,0x00);
	}
	GPIO_H(CSN_PORT);
	return status;
}
void receive_init()
{
	Receive.init = FALSE;
	Receive.mode = TRUE;
	Receive.flag = FALSE;
}
u8 receive_function()
{
	u8 i;
	if(Receive.init)
	{
		Receive.init = FALSE;
		Receive.mode = TRUE;
		nrf_receive_mode(10,15);
	}

	if(Receive.mode)//one min once
	{
		if(IRQ_STAREREAD == 0 )
		{
			if((SPI_RW_Reg(0x07,SPI1,0x00)&0x40) == 0x40)
			{
				memset(redata,0,sizeof(redata));
				receive_data(0x61,0x0f,redata);
				spi_write(FLUSH_RX);
				SPI_RW_Reg(WRITE_REG + STATUS,SPI1,0xFF);
			}
			if(redata[1] == 0)//head
			{
				memset(redata,0,sizeof(redata));
				KeyFlag.head = TRUE;
				Receive.mode = FALSE;
			}
			else if(redata[1] == 0xb2 )//send
			{
				InfoPack.data[0] = redata[8];
				KeyFlag.send = TRUE;
				memset(redata,0,sizeof(redata));
				Receive.mode = FALSE;
			}
		}
	}
}



















