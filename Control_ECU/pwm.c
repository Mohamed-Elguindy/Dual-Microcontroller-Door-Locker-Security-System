/*
 * pwm.c
 *
 *  Created on: Oct 5, 2024
 *      Author: MOH
 */
#include "avr/io.h"
#include "common_macros.h"
#include "pwm.h"
#include "gpio.h"
void Timer0_INIT_PWM(void){
	OCR0  = 0; //Set Compare value
	TCNT0 = 0;
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	 TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00) | (1 << CS01);
}
void PWM_Timer0_Start(uint8 duty_cycle) {
	OCR0  = duty_cycle*2.55; //Set Compare value
}


