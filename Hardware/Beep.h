#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"                  // Device header

void Beep_Init(void);
void Beep_Set(int8_t Cmd);
void Beep_Turn(void);

#endif
