#include "batterie.h"

#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config

#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"

void init_surv_batterie(){
	// RCC GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	LL_GPIO_SetPinMode(GPIOC, 2, LL_GPIO_MODE_FLOATING);
	ADC1->CR2 |= ADC_CR2_ADON;
}

int batterie_OK(){
	// Selection du channel 12
	ADC1->SQR3 = 0 ;
	ADC1->SQR3 |= (1<<3) | (1<< 2);
	//ADON -> 1
	ADC1->CR2 |= ADC_CR2_ADON;
	// Diminution de la fréquence de conversion
	ADC1->SMPR1 |= (7<<6) ;
	//wait EOC
	while((ADC1->SR & ADC_SR_EOC) == 1){}
	unsigned int niv_batterie = ADC1->DR;;	
	return (niv_batterie > 0x0700);
}
