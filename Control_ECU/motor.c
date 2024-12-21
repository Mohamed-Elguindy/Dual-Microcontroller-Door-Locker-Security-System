/*
 * motor.c
 *
 *  Created on: Oct 5, 2024
 *      Author: MOH
 */
#include "common_macros.h"
#include "gpio.h"
#include "motor.h"
#include "pwm.h"
void DcMotor_Init(void){
	GPIO_setupPinDirection(MOTOR_PORT_ID1,MOTOR_PIN_ID1,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID2,MOTOR_PIN_ID2,PIN_OUTPUT);

	GPIO_writePin(MOTOR_PORT_ID1,MOTOR_PIN_ID1,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID2,MOTOR_PIN_ID2,LOGIC_LOW);

	GPIO_setupPinDirection(PORTB_ID,PIN3_ID, PIN_OUTPUT);

}
void DcMotor_Rotate(DcMotor_State state, uint8 speed){


    switch (state) {
        case ACW:
            GPIO_writePin(MOTOR_PORT_ID1, MOTOR_PIN_ID1, LOGIC_HIGH);
            GPIO_writePin(MOTOR_PORT_ID2, MOTOR_PIN_ID2, LOGIC_LOW);
        	PWM_Timer0_Start(speed);
            break;

        case CW:
            GPIO_writePin(MOTOR_PORT_ID1, MOTOR_PIN_ID1, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT_ID2, MOTOR_PIN_ID2, LOGIC_HIGH);
        	PWM_Timer0_Start(speed);
            break;

        case STOP:
            GPIO_writePin(MOTOR_PORT_ID1, MOTOR_PIN_ID1, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT_ID2, MOTOR_PIN_ID2, LOGIC_LOW);
            PWM_Timer0_Start(0);
            break;
    }


}
