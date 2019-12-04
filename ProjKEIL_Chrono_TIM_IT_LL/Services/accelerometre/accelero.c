#include "accelero.h"

#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config

#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"

void init_accelero(){
	// RCC GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	LL_GPIO_SetPinMode(GPIOC, 0, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOC, 1, LL_GPIO_MODE_FLOATING);
	
	// RCC ADC1
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	LL_ADC_InitTypeDef structure = { 0x00000000U,0};
	LL_ADC_Init(ADC1,&structure);
	
	//ADON -> 1
	ADC1->CR2 |= ADC_CR2_ADON;
}

int angleSafe(){
	// Selection du channel 10
	ADC1->SQR3  = 0;
	ADC1->SQR3 |= (1<<3) | (1<< 1);
	//ADON -> 1
	ADC1->CR2 |= ADC_CR2_ADON;
	// Diminution de la fréquence de conversion
	ADC1->SMPR1 |= 5;
	//wait EOC
	while((ADC1->SR & ADC_SR_EOC) == 1){}
	unsigned int angle = ADC1->DR; //& 0x0CFF;	
		//for (int j= 0; j < 1000; j++);
	return ((angle<0x800) && (angle>0x5C0));
}
