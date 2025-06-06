#include "stm32f10x.h"                  // Device header
#include "LED.h"

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	LED_OFF();
}


void LED_ON(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}


void LED_OFF(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}


void LED_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8) == 0)
	{
		LED_OFF();
	}
	else
	{
		LED_ON();
	}
}
