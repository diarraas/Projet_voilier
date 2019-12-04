#ifndef SERVOM_H
#define SERVOM_H

#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "MyTimer.h"

void init_servom() ;

void bordageVoiles(int angle_girouette) ;

#endif