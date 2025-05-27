#ifndef __HX711_H
#define	__HX711_H

#include "stm32f10x.h"
#include "Delay.h"
#include <math.h>


// HX711 GPIO宏定义
#define		HX711_GPIO_CLK				RCC_APB2Periph_GPIOA
#define		HX711_SCK_GPIO_PORT			GPIOA
#define		HX711_SCK_GPIO_PIN			GPIO_Pin_2
#define		HX711_DT_GPIO_PORT			GPIOA
#define		HX711_DT_GPIO_PIN			GPIO_Pin_3

#define		HX711_SCK_H					GPIO_SetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);
#define		HX711_SCK_L					GPIO_ResetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);

#define		HX711_DT					GPIO_ReadInputDataBit(HX711_DT_GPIO_PORT, HX711_DT_GPIO_PIN)


void HX711_Init(void);
unsigned long HX711_GetData(void);

#endif
