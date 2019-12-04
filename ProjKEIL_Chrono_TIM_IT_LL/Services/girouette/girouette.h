#ifndef GIROUETTE_H
#define GIROUETTE_H



#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_exti.h"
#include "MyTimer.h"


void init_girouette() ;
int angle_girouette() ;
void Init_Origin_Girouette(TIM_TypeDef * Timer, uint32_t Port, uint32_t Line) ;

#endif