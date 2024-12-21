/*
 * timer.c
 *
 *  Created on: Oct 24, 2024
 *      Author: MOH
 */
#include "common_macros.h"
#include "gpio.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
static void (*g_timer0_callBackPtr)(void) = NULL_PTR;
static void (*g_timer1_callBackPtr)(void) = NULL_PTR;
static void (*g_timer2_callBackPtr)(void) = NULL_PTR;
void Timer_init(const Timer_ConfigType * Config_Ptr){
	switch(Config_Ptr->timer_ID){
	case TIMER0:
		TCCR0 |= Config_Ptr->timer_clock|Config_Ptr->timer_mode|(1<<FOC0);
		TCNT0=Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode==COMPARE){
			TIMSK|=(1<<OCIE0);
			OCR0 = Config_Ptr->timer_compare_MatchValue;
		}
		else if(Config_Ptr->timer_mode==NORMAL)
			TIMSK|=(1<<TOIE0);
		break;
	case TIMER2:
		TCCR2 |= Config_Ptr->timer_clock|Config_Ptr->timer_mode|(1<<FOC2);
		TCNT2=Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode==COMPARE){
			TIMSK|=(1<<OCIE2);
			OCR2 = Config_Ptr->timer_compare_MatchValue;
		}
		else if(Config_Ptr->timer_mode==NORMAL)
			TIMSK|=(1<<TOIE2);
		break;
	case TIMER1:
		TCCR1B |=Config_Ptr->timer_clock|Config_Ptr->timer_mode;
        TCCR1A |= (1 << FOC1A) | (1 << FOC1B);
		TCNT1=Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode==COMPARE){
			TIMSK|=(1<<OCIE1A);
			OCR1A = Config_Ptr->timer_compare_MatchValue;
		}
		else if(Config_Ptr->timer_mode==NORMAL)
			TIMSK|=(1<<TOIE1);
	}
}
void Timer_deInit(Timer_ID_Type timer_type) {
    switch (timer_type) {
        case TIMER0:
            TCCR0 = 0;
            TCNT0 = 0;
            TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
            g_timer0_callBackPtr = NULL_PTR;
            break;

        case TIMER1:
            TCCR1A = 0;
            TCCR1B = 0;
            TCNT1 = 0;
            TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A));
            g_timer1_callBackPtr = NULL_PTR;
            break;

        case TIMER2:
            TCCR2 = 0;
            TCNT2 = 0;
            TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
            g_timer2_callBackPtr = NULL_PTR;
            break;
    }
}
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID) {
    switch (a_timer_ID) {
        case TIMER0:
            g_timer0_callBackPtr = a_ptr;
            break;

        case TIMER1:
            g_timer1_callBackPtr = a_ptr;
            break;

        case TIMER2:
            g_timer2_callBackPtr = a_ptr;
            break;
    }
}

ISR(TIMER0_OVF_vect) {
    if (g_timer0_callBackPtr != NULL_PTR) {
        (*g_timer0_callBackPtr)();
    }
}

ISR(TIMER0_COMP_vect) {
    if (g_timer0_callBackPtr != NULL_PTR) {
        (*g_timer0_callBackPtr)();
    }
}

ISR(TIMER1_OVF_vect) {
    if (g_timer1_callBackPtr != NULL_PTR) {
        (*g_timer1_callBackPtr)();
    }
}

ISR(TIMER1_COMPA_vect) {
    if (g_timer1_callBackPtr != NULL_PTR) {
        (*g_timer1_callBackPtr)();
    }
}

ISR(TIMER2_OVF_vect) {
    if (g_timer2_callBackPtr != NULL_PTR) {
        (*g_timer2_callBackPtr)();
    }
}

ISR(TIMER2_COMP_vect) {
    if (g_timer2_callBackPtr != NULL_PTR) {
        (*g_timer2_callBackPtr)();
    }
}





