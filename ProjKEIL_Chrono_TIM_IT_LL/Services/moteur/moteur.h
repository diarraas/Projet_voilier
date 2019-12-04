// RIEN A MODIFIER //

#ifndef MOTEUR_H
#define MOTEUR_H

#include "stm32f103xb.h" 


void config_gpio_moteur(void);

void set_vitesse_moteur(int v);

void set_sens_moteur(int s);

#endif
