#include "servomoteur.h"

void init_servom() {
	
	//Configuer Arr et Psc pour avoir une periode de 20 ms
	int Arr = 7200;
	int Psc = 199;
	
	//Initialisation du Timer1 Ch1 en mode PWM
	MyTimer_Conf_PWM(TIM1,1,Arr,Psc) ;
	// Activation CEN
	LL_TIM_EnableCounter(TIM1); 
	// Activation CC1E
	TIM1->CCER &= 0xFFFE;
	TIM1->CCER += 1 ;
	//Activation MOE (SPECIFIQUE TIM1)
	TIM1->BDTR &= 0x7FFF;
	TIM1->BDTR += 0x8000;
	
	/* Activation de l'horloge pour GPIOA */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	/* Configuration du port GPIOA8 en Alternate Output */
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE );
	
}

// Pour agir sur le bordage, il faut agir sur TIM1_CCR1
// -> TIM1_CCR1 = 1ms <=> Bordage des voiles à 0
// -> TIM1_CCR1 = 2ms <=> Bordage des voiles à 90
// TIM1->CCR1 = (7200*5)/100) ; => regle a 1ms 
void bordageVoiles(int angle_girouette) {
	if ((angle_girouette <= 45) || (angle_girouette >= (360-45)) ){
		TIM1->CCR1 = (int)((7200.*5.)/100.) ;
	} else {
		if (angle_girouette > 45 && angle_girouette < 180){
			int angle_voiles = (int)(((7200.*5.)/100.) + ((float)(angle_girouette-45))*((7200.*5.)/100.)/135.) ;
			TIM1->CCR1 = angle_voiles ;
		}else if (angle_girouette > 180 && angle_girouette < (360-45)) {
			int angle_voiles = (int)(((7200.*5.)/100.) + (315.-(float)(angle_girouette))*((7200.*5.)/100.)/135.) ;
			TIM1->CCR1 = angle_voiles ;
		}
	}
}