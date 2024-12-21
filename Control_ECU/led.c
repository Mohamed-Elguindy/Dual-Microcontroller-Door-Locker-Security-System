/*
 * led.c
 *
 *  Created on: Oct 5, 2024
 *      Author: MOH
 */
#include "common_macros.h"
#include "gpio.h"
#include "led.h"
void LEDS_init(void){
	GPIO_setupPinDirection(RED_LED_PORT_ID,RED_LED_PIN_ID,PIN_OUTPUT);
    GPIO_writePin(RED_LED_PORT_ID, RED_LED_PIN_ID,LOGIC_LOW);

	GPIO_setupPinDirection(BLUE_LED_PORT_ID,BLUE_LED_PIN_ID,PIN_OUTPUT);
    GPIO_writePin(BLUE_LED_PORT_ID, BLUE_LED_PIN_ID,LOGIC_LOW);

	GPIO_setupPinDirection(GREEN_LED_PORT_ID,GREEN_LED_PIN_ID,PIN_OUTPUT);
    GPIO_writePin(GREEN_LED_PORT_ID, GREEN_LED_PIN_ID,LOGIC_LOW);
}
void LED_on(LED_ID id)
{
	switch(id)
	{
		case LED_RED:
			GPIO_writePin(RED_LED_PORT_ID, RED_LED_PIN_ID, LED_ON);
			break;
		case LED_GREEN:
			GPIO_writePin(GREEN_LED_PORT_ID, GREEN_LED_PIN_ID, LED_ON);
			break;
		case LED_BLUE:
			GPIO_writePin(BLUE_LED_PORT_ID, BLUE_LED_PIN_ID, LED_ON);
			break;
	}
}

void LED_off(LED_ID id)
{
	switch(id)
	{
		case LED_RED:
			GPIO_writePin(RED_LED_PORT_ID, RED_LED_PIN_ID, LED_OFF);
			break;
		case LED_GREEN:
			GPIO_writePin(GREEN_LED_PORT_ID, GREEN_LED_PIN_ID, LED_OFF);
			break;
		case LED_BLUE:
			GPIO_writePin(BLUE_LED_PORT_ID, BLUE_LED_PIN_ID, LED_OFF);
			break;
	}
}
