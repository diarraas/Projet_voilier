// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Chrono.h"
#include "MyTimer.h"
#include "Emetteur.h"

#include <stm32f1xx_ll_usart.h>
#include <stm32f1xx_ll_gpio.h>

void config_uart(){

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	LL_GPIO_InitTypeDef gpio;
	
	
	// broche de commande du tranceiver
	gpio.Pin = LL_GPIO_PIN_11;
	gpio.Mode = LL_GPIO_MODE_OUTPUT;
	gpio.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pull = LL_GPIO_PULL_DOWN;
	
	LL_GPIO_Init(GPIOA, &gpio);
	
	
	// broche du TX de l'uart
	gpio.Pin = LL_GPIO_PIN_9;
	gpio.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOA, &gpio);
	
	LL_USART_InitTypeDef USART_InitStruct;
	
	LL_USART_Enable(USART1);

 LL_USART_StructInit(&USART_InitStruct); 
	

	LL_USART_SetBaudRate(USART1,72000000,9600);
	LL_USART_Init (USART1, &USART_InitStruct);
	LL_USART_EnableDirectionTx (USART1);

	
	
	
}

void send_char(char a){
	
	while(LL_USART_IsActiveFlag_TXE(USART1))
		LL_USART_TransmitData8(USART1,a);
	while(!LL_USART_IsActiveFlag_TC(USART1));
}	

void send_msg(char * msg){
	
	int i = 0;
	
	while( i < strlen(msg)) {
		send_char(msg[i]);
		i++;
	}
}
