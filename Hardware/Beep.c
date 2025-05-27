#include "stm32f10x.h"                  // Device header
#include "Beep.h"

void Beep_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Beep_Set(0);
}


void Beep_Set(int8_t Cmd)
{
	if (Cmd) {GPIO_SetBits(GPIOB, GPIO_Pin_13);}
	else {GPIO_ResetBits(GPIOB, GPIO_Pin_13);}
}

void Beep_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_13) == 0)
	{
		Beep_Set(0);
	}
	else
	{
		Beep_Set(1);
	}
}

