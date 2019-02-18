/************Copyright(C) Kaikai Technology 2019-02-14***********************
File name  : nrf.c
Description: 主要实现nrf初始化及发功功能
Platform   : IAR V2.20.3
Version    : V1.0
Author	   : Jason
Create Time: 2019-02-16
Modify	   : 
Modify Time: 
******************************************************************************/

#include "nrf.h"
#include "gpio.h"
#include "delay.h"

u8 TX_ADDRESS[5]  =  {0xD2,0x52,0x6E,0x10,0x01};
u8 RX_ADDRESS[5] =   {0x34,0x43,0x10,0x10,0x01};
//spi总线初始化
void SPI24r1_Init(void)
{
    SI24r1_Gpioinit();
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1,ENABLE);
    SPI_DeInit(SPI1);  
    SPI_Init( SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_16, SPI_Mode_Master, 
            SPI_CPOL_Low, SPI_CPHA_1Edge,SPI_Direction_2Lines_FullDuplex,
            SPI_NSS_Soft, 0x07);  
    SPI_Cmd(SPI1, ENABLE);
}
u8  SPI_Write_Buf(u8 reg, SPI_TypeDef* SPIx,u8 *pBuf, u8 u8s)
{
    u8  status,u8_ctr;
    GPIO_L(CSN_PORT);  
    status=SPI_SendByte(SPIx,reg);
    for(u8_ctr=0; u8_ctr<u8s; u8_ctr++) // then write all u8 in buffer(*pBuf)
    {
    	SPI_SendByte(SPIx,*pBuf++);
    }
    GPIO_H(CSN_PORT);
    return(status);
}
u8  SPI_RW_Reg(u8 reg,SPI_TypeDef* SPIx, u8 value)
{
    u8  status;
    GPIO_L(CSN_PORT);
    status=SPI_SendByte(SPIx,reg);
    SPI_SendByte(SPIx,value);
    GPIO_H(CSN_PORT);
    return status;
}
void nrfsend(u8 *pBuf,u8 ch)
{  
    GPIO_L(CE_PORT);
    SPI_RW_Reg(WRITE_REG + CONFIG,SPI1, 0x08);     //CRC16
    SPI_RW_Reg(WRITE_REG + EN_AA ,SPI1,0x01);//Compatible Nodic:0x00
    SPI_RW_Reg(WRITE_REG + EN_RXADDR ,SPI1,0x01);//Reserved
    SPI_RW_Reg(WRITE_REG + SETUP_AW,SPI1,0x01);//Send Address width:3u8
    SPI_RW_Reg(WRITE_REG + SETUP_RETR,SPI1,0x5a);//Reserved
    SPI_RW_Reg(WRITE_REG + RF_SETUP,SPI1,0x0f); //0x26//0x27 // 0dBm, 1Mbps
    
    SPI_Write_Buf(WRITE_REG + TX_ADDR,SPI1,TX_ADDRESS, TX_ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,SPI1,TX_ADDRESS, TX_ADR_WIDTH);
    SPI_RW_Reg(WRITE_REG + STATUS,SPI1,0xFF);
    SPI_Write_Buf(WR_TX_PLOAD,SPI1,pBuf,0x0f);
    delay_us(100);
    GPIO_L(CE_PORT);
    SPI_RW_Reg(WRITE_REG + RF_CH,SPI1,ch);	   //RF Channel:2460MHz
    SPI_RW_Reg(WRITE_REG + CONFIG,SPI1,0x0A);// TX Interruption:Enable,CRC16,Power on
    GPIO_H(CE_PORT);
    while(IRQ_STAREREAD !=0);
}
void SleepMode()
{  
    GPIO_L(CE_PORT);
    SPI_RW_Reg(WRITE_REG + STATUS,SPI1,0xFF);   
    SPI_RW_Reg(WRITE_REG + CONFIG,SPI1,0x08);//Power off
}
u8 SPI_Read_Reg(SPI_TypeDef* SPIx,u8 reg)
{
    u8 value;
    GPIO_L(CSN_PORT); 
    SPI_SendByte(SPIx,reg);//发送寄存器值(位置),并读取状态值
    value = SPI_SendByte(SPIx,0XFF);
    GPIO_H(CSN_PORT);
    return value;
}
void Sleep()
{
	SleepMode();
	SPI_DeInit(SPI1);
	CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);
	SPI_Cmd(SPI1, DISABLE);  /* 使能SPI */
}















