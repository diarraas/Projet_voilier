// RIEN A MODIFIER //

#ifndef EMETTEUR_H
#define EMETTEUR_H

#include "stm32f103xb.h" 


void config_uart(void);

void send_char(char a);

void send_msg(char * msg);

#endif
