// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 

#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 


// variable pointeur de fonction permettant de mémoriser le callback à appeler depuis
// le handler d'IT
void (*Ptr_ItFct_TIM1)(void); 
void (*Ptr_ItFct_TIM2)(void); 
void (*Ptr_ItFct_TIM3)(void); 
void (*Ptr_ItFct_TIM4)(void); 




/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void MyTimer_Conf_encoder(TIM_TypeDef * Timer,int Arr, int Psc)
{
	 LL_TIM_ENCODER_InitTypeDef My_LL_Tim_Init_Struct;
	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	// chargement structure Arr, Psc, Up Count
	My_LL_Tim_Init_Struct.EncoderMode=LL_TIM_ENCODERMODE_X4_TI12;
	My_LL_Tim_Init_Struct.IC1Polarity= LL_TIM_IC_POLARITY_RISING ;
	My_LL_Tim_Init_Struct.IC1ActiveInput= LL_TIM_ACTIVEINPUT_DIRECTTI;
	My_LL_Tim_Init_Struct.IC1Prescaler= Psc;
	My_LL_Tim_Init_Struct.IC1Filter= LL_TIM_IC_FILTER_FDIV1;
	My_LL_Tim_Init_Struct.IC2Polarity= LL_TIM_IC_POLARITY_RISING ;
	My_LL_Tim_Init_Struct.IC2ActiveInput= LL_TIM_ACTIVEINPUT_INDIRECTTI;
	My_LL_Tim_Init_Struct.IC2Prescaler= Psc;
	My_LL_Tim_Init_Struct.IC2Filter= LL_TIM_IC_FILTER_FDIV1;
	
	// Initialisation de la structure
	LL_TIM_ENCODER_Init(Timer,&My_LL_Tim_Init_Struct);
	
	// Configure l'ARR du timer
	LL_TIM_SetAutoReload (Timer, Arr) ;
		
}

void MyTimer_Conf_PWM(TIM_TypeDef * Timer, int Channel, int Arr, int Psc)
{
	LL_TIM_OC_InitTypeDef My_LL_Tim_Init_Struct_PWM;
	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// chargement structure
	My_LL_Tim_Init_Struct_PWM.OCMode = LL_TIM_OCMODE_PWM1 ;
	My_LL_Tim_Init_Struct_PWM.OCState = LL_TIM_OCSTATE_DISABLE  ;
	My_LL_Tim_Init_Struct_PWM.OCNState = LL_TIM_OCSTATE_DISABLE  ;
	//My_LL_Tim_Init_Struct_PWM.CompareValue =
	My_LL_Tim_Init_Struct_PWM.OCPolarity = LL_TIM_OCPOLARITY_HIGH ;
	My_LL_Tim_Init_Struct_PWM.OCNPolarity = LL_TIM_OCPOLARITY_HIGH ;
	
	//My_LL_Tim_Init_Struct_PWM.OCIdleState = LL_TIM_OCIDLESTATE_LOW  ;
	//My_LL_Tim_Init_Struct_PWM.OCNIdleState = LL_TIM_OCIDLESTATE_LOW  ;	
	
	LL_TIM_OC_Init(Timer,Channel,&My_LL_Tim_Init_Struct_PWM);
	
	LL_TIM_CC_EnableChannel(Timer, Channel);
	
	
	
	// Configure l'ARR du timer
	LL_TIM_SetAutoReload (Timer, Arr) ;
	
	// Configure le Psc du timer
	LL_TIM_SetPrescaler(Timer,Psc);
	
	LL_TIM_EnableCounter(Timer);
}

/**
	* @brief  Démarre le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer)
{
		LL_TIM_EnableCounter(Timer);
}

/**
	* @brief  Arrêt le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer)
{
	LL_TIM_DisableCounter(Timer);
}


/**
	* @brief  Configure le Timer considéré en interruption sur débordement.
  * @note   A ce stade, les interruptions ne sont pas validés (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction à lancer sur interruption
	*         int Prio : priorité associée à l'interruption
  * @retval None
  */
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio)
{
	// affectation de la fonction
	if (Timer==TIM1) Ptr_ItFct_TIM1=IT_function;
	else if (Timer==TIM2)	Ptr_ItFct_TIM2=IT_function;
	else if (Timer==TIM3)	Ptr_ItFct_TIM3=IT_function;
	else  Ptr_ItFct_TIM4=IT_function;

	
	// Blocage IT (il faudra la débloquer voir fct suivante)
	LL_TIM_DisableIT_UPDATE(Timer);
	
	// validation du canal NVIC
	IRQn_Type TIM_irq;
	
	if (Timer==TIM1) TIM_irq=TIM1_UP_IRQn;
	else if (Timer==TIM2)	TIM_irq=TIM2_IRQn;
	else if (Timer==TIM3)	TIM_irq=TIM3_IRQn;
	else 	TIM_irq=TIM4_IRQn;
	
	NVIC_SetPriority(TIM_irq, Prio);
	NVIC_EnableIRQ(TIM_irq);

	
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Enable(TIM_TypeDef * Timer)
{
		LL_TIM_EnableIT_UPDATE(Timer);
}


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Disable(TIM_TypeDef * Timer)
{
			LL_TIM_DisableIT_UPDATE(Timer); 
}	


/*
============ LES INTERRUPTIONS =================================

*/

void TIM1_UP_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM1);
	(*Ptr_ItFct_TIM1)();
}	

void TIM2_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM2);
	(*Ptr_ItFct_TIM2)();
}	

void TIM3_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM3);
	(*Ptr_ItFct_TIM3)();
}	

void TIM4_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM4);
	(*Ptr_ItFct_TIM4)();
}	
