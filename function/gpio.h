#ifndef _gpio_h
#define _gpio_h
#include "stm8l15x_gpio.h"

#define CE_PORT         GPIOF
#define CE_PORT_PIN     GPIO_Pin_0
#define CSN_PORT        GPIOE
#define CSN_PORT_PIN    GPIO_Pin_7
#define SCK_PORT        GPIOB
#define SCK_PORT_PIN    GPIO_Pin_5
#define MOSI_PORT       GPIOB
#define MOSI_PORT_PIN   GPIO_Pin_6
#define MISO_PORT       GPIOB
#define MISO_PORT_PIN   GPIO_Pin_7
#define IRQ_PORT        GPIOC
#define IQR_PORT_PIN    GPIO_Pin_4

#define GPIO_H(x)       GPIO_SetBits(x,x##_PIN)
#define GPIO_L(x)       GPIO_ResetBits(x,x##_PIN)
#define IRQ_STAREREAD   GPIO_ReadInputDataBit(IRQ_PORT, IQR_PORT_PIN)

void SI24r1_Gpioinit(void);
void gpio_inti(void);


#endif








