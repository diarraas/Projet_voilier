// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/


#include <stdio.h>
#include <stdlib.h>
#include "Chrono.h"
#include "MyTimer.h"
#include "telecommande.h"
#include "stm32f1xx_ll_gpio.h"

void config_gpio_telecommande(){
	
	//enable clock gpio
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	//PB6 en floating input
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_6,LL_GPIO_MODE_FLOATING);
	
	
}


void init_timer_pwm_input(TIM_TypeDef * Timer){
	
	if (Timer == TIM1)	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	if (Timer == TIM2)	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	if (Timer == TIM3)	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	if (Timer == TIM4)	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
/*
	• Select the active input for TIMx_CCR1: write the CC1S bits to 01 in the TIMx_CCMR1
register (TI1 selected).
• Select the active polarity for TI1FP1 (used both for capture in TIMx_CCR1 and counter
clear): write the CC1P to ‘0’ (active on rising edge).
• Select the active input for TIMx_CCR2: write the CC2S bits to 10 in the TIMx_CCMR1
register (TI1 selected).
• Select the active polarity for TI1FP2 (used for capture in TIMx_CCR2): write the CC2P
bit to ‘1’ (active on falling edge).
• Select the valid trigger input: write the TS bits to 101 in the TIMx_SMCR register
(TI1FP1 selected).
• Configure the slave mode controller in reset mode: write the SMS bits to 100 in the
TIMx_SMCR register.
• Enable the captures: write the CC1E and CC2E bits to ‘1 in the TIMx_CCER register.
	*/
	
	Timer->ARR = 0xFFFF;
	Timer->PSC = 0x0100;
	
	Timer->CR1 |= TIM_CR1_CEN;
	
	
	Timer->CCMR1 |= TIM_CCMR1_CC1S_0;
	Timer->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	
	Timer->CCER &= ~TIM_CCER_CC1P;	
	
	Timer->CCMR1 &= ~TIM_CCMR1_CC2S_0;
	Timer->CCMR1 |= TIM_CCMR1_CC2S_1;
	
	Timer->CCER |= TIM_CCER_CC2P; 
	
	Timer->SMCR |= TIM_SMCR_TS_0;
	Timer->SMCR &= ~TIM_SMCR_TS_1;
	Timer->SMCR |= TIM_SMCR_TS_2;
	
	Timer->SMCR &= ~TIM_SMCR_SMS_0;
	Timer->SMCR &= ~TIM_SMCR_SMS_1;
	Timer->SMCR |= TIM_SMCR_SMS_2;
	
	Timer->CCER |= TIM_CCER_CC1E;
	Timer->CCER |= TIM_CCER_CC2E;
	
}

int get_period_telecommande(TIM_TypeDef * Timer){
	return(Timer->CCR2);	//per
}

int get_angle_telecommande(int per){	
	return(1.57*per - 690.4 +15);
}

float get_vitesse_telecommande(int angle){	
	return((5./9.)*abs(angle));	
}

int get_sens_telecommande(int angle){
	if (angle <0) return(-1);
	else return(1);
}

