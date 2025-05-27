#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Motor.h"
#include "LED.h"
#include "OLED.h"
#include "AD.h"
#include "hx711.h"
#include "Beep.h"
#include "Trigger.h"

uint16_t TempADValue;
float TempVoltage;
uint16_t BatteryADValue;
float BatteryVoltage;

int WeightValue;
int weight;
int32_t reset;
float Weights=100.0;			// 100g
int32_t Weights_100=8493860;	// 100g

uint8_t Temp_Judge = 0;
uint8_t Weight_Judge = 0;
uint8_t Battery_Judge = 0;

uint8_t TimerCount = 0;
uint8_t Counter = 0;
uint8_t Weight_Count = 0;
uint8_t Trigger_Count = 0;

int main(void)
{
	LED_Init();
	OLED_Init();
	Timer_Init();
	AD_Init();
	HX711_Init();
	Beep_Init();
	Motor_Init();
	Trigger_Init();
	
	OLED_ShowString(1, 1, "Temp:");
	OLED_ShowString(2, 1, "Battery:0.00V");
	OLED_ShowString(3, 1, "Weight:");
	OLED_ShowString(4, 1, "Trigger is");
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	while (1)
	{
		OLED_ShowNum(2, 9, BatteryVoltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(BatteryVoltage * 100) % 100, 2);
		OLED_ShowNum(3, 8, weight, 6);
		uint8_t Trigger_State = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15);
		if (Trigger_State)
		{
			OLED_ShowString(4, 12, "OFF");
		}
		else
		{
			OLED_ShowString(4, 12, "ON ");
		}
		
		
		if (TempVoltage < 1.32)
		{
			Temp_Judge = 0;
			Battery_Judge = 1;
			OLED_ShowString(1, 6, "Extreme");
		}
		else if (TempVoltage <= 1.45)
		{
			Temp_Judge = 1;
			Battery_Judge = 0;
			OLED_ShowString(1, 6, "High   ");
		}
		else
		{
			Temp_Judge = 0;
			Battery_Judge = 0;
			OLED_ShowString(1, 6, "Fine   ");
		}
		if (weight >= 105)
		{
			Weight_Judge = 1;
		}
		else Weight_Judge = 0;
		
		if (BatteryVoltage >= 3.29)
		{
			Trigger_Set(0);
			Trigger_Count = 1;
		}
	}
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		// 清除TIM2更新事件的中断标志位
		TempADValue = AD_GetValue(ADC_Channel_0);
		TempVoltage = (float)TempADValue / 4095 * 3.3;
		BatteryADValue = AD_GetValue(ADC_Channel_1);
		BatteryVoltage = (float)BatteryADValue / 4095 * 3.3;
		WeightValue = HX711_GetData();
		weight=(float)(WeightValue-reset)*Weights/(float)(Weights_100-reset);
	}
}


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		// 清除TIM3更新事件的中断标志位
		TimerCount++;
		if (TimerCount == 2)
		{
			TimerCount = 0;
			Counter = ~Counter;
			if (Temp_Judge == 1 && Battery_Judge == 0 && Trigger_Count == 0)
			{
				if (Counter)
				{
					Beep_Set(1);
					LED_ON();
				}
				else
				{
					Beep_Set(0);
					LED_OFF();
				}
			}
			else if (Temp_Judge == 0 && Battery_Judge == 0  && Trigger_Count == 0)
			{
				Beep_Set(0);
				LED_OFF();
				Trigger_Set(1);
			}
			
			if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 1  && Trigger_Count == 0)
			{
				Weight_Count++;
				if (Weight_Count >= 80) Weight_Count = 80;
				if (Weight_Count >= 20 && Weight_Count <= 40)
				{
					if (Counter)
					{
						Beep_Set(1);
						LED_ON();
					}
					else
					{
						Beep_Set(0);
						LED_OFF();
					}
				}
			}
			else if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 0  && Trigger_Count == 0)
			{
				Weight_Count = 0;
				Beep_Set(0);
				LED_OFF();
			}
		}

		if (Battery_Judge == 0 && Temp_Judge == 1  && Trigger_Count == 0)
		{
			if (TimerCount)
			{
				Motor_SetSpeed(1);
			}
			else
			{
				Motor_SetSpeed(0);
			}
		}
		
		if (Battery_Judge == 1 && Temp_Judge == 0  && Trigger_Count == 0)
		{
			Trigger_Set(0);
			Motor_SetSpeed(1);
			if (TimerCount)
			{
				Beep_Set(1);
				LED_ON();
			}
			else
			{
				Beep_Set(0);
				LED_OFF();
			}
		}
		else if (Battery_Judge == 0 && Temp_Judge == 0 && Weight_Judge == 0)
		{
			Motor_SetSpeed(0);
			Beep_Set(0);
			LED_OFF();
		}
		
		if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 1)
		{
			Weight_Count++;
			if (Weight_Count >= 80) Weight_Count = 80;
			if (Weight_Count >= 40 && Weight_Count <= 60)
			{
				if (TimerCount)
				{
					Beep_Set(1);
					LED_ON();
				}
				else
				{
					Beep_Set(0);
					LED_OFF();
				}
			}
			else if (Weight_Count >= 80)
			{
				Trigger_Set(0);
				Beep_Set(1);
				LED_ON();
			}
		}
		
		else if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 0  && Trigger_Count == 0)
		{
			Weight_Count = 0;
			Beep_Set(0);
			LED_OFF();
		}
		
		if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 0  && Trigger_Count == 1)
		{
			if (TimerCount)
			{
				Beep_Set(1);
				LED_ON();
			}
			else
			{
				Beep_Set(0);
				LED_OFF();
			}
		}
		else if (Temp_Judge == 0 && Battery_Judge == 0 && Weight_Judge == 0  && Trigger_Count == 0)
		{
			Weight_Count = 0;
			Beep_Set(0);
			LED_OFF();
		}
	}
}
