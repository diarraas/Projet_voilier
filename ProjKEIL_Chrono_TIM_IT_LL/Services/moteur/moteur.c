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
#include "moteur.h"
#include "stm32f1xx_ll_gpio.h"

void config_gpio_moteur(){
	
	//enable clock gpio
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//PA1 en output pushpull
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	
	//PA2 en output pushpull
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	
}

void set_vitesse_moteur(int v){
	float pourcent = ((float)v/100.0);
	int dc = (int)(pourcent*99.0);
	TIM2 -> CCR2 = dc;	
}

void set_sens_moteur(int sens){
	if(sens == -1)	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_2);
	else LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_2);
}
