/* Commande des voiles */
/* Acteur : Servo_Moteur */

#include "girouette.h"

void init_girouette() {
	
	// Initialisation du timer TIM3 en capture compare
	int Arr = (360*4)-1;
	int Psc = 0 ;
	MyTimer_Conf_encoder(TIM3,Arr,Psc) ;
	
	/* Activation de l'horloge pour GPIOA */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Initialisation des GPIOA 5 (Index) et GPIOA 6 (CHA) et GPIOA 7 (CHB)
	LL_GPIO_SetPinMode(GPIOA,5,LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA,6,LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA,7,LL_GPIO_MODE_FLOATING);
	
	// Remise a zero pour la girouette
	while((LL_GPIO_ReadInputPort(GPIOA) & ((0x1)<<5)) == 0) {}
	// Activation du compteur du timer 3
	LL_TIM_EnableCounter(TIM3);
}

int angle_girouette() {
	return LL_TIM_GetCounter(TIM3)/4 ;
}