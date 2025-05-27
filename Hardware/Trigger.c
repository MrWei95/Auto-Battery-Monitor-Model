#include "stm32f10x.h"                  // Device header
#include "Trigger.h"
#include "OLED.h"

void Trigger_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	Trigger_Set(0);
}


void Trigger_Set(int8_t Cmd)
{
	if (Cmd)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
//		OLED_ShowString(4, 12, "ON ");
	}
	else
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
//		OLED_ShowString(4, 12, "OFF");
	}
}
