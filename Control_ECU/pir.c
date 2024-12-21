/*
 * pir.c
 *
 *  Created on: Oct 25, 2024
 *      Author: MOH
 */
#include "common_macros.h"
#include "gpio.h"
#include "pir.h"
void PIR_init(void){
	GPIO_setupPinDirection(PIR_PORT,PIR_PIN,PIN_INPUT);
}
uint8 PIR_getState(void){
	return GPIO_readPin(PIR_PORT,PIR_PIN);
}
