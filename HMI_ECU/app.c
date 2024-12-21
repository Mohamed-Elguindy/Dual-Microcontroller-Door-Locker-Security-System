/*
 * app.c
 *
 *  Created on: Oct 25, 2024
 *      Author: MOH
 */
#include "common_macros.h"
#include "gpio.h"
#include "timer.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define pass_size 5
#define enter_button 13
uint8 pass1[pass_size];
uint8 pass2[pass_size];
uint8 pass3[pass_size];
uint8 pass3_1[pass_size];
uint8 pass3_2[pass_size];
uint8 change_pass[pass_size];
uint8 Alarm_flag=0;
uint8 step=1;
uint8 move=0;
uint8 wrong_pass_counter=0;
uint8 c_wrong_pass_counter=0;
volatile uint8 tick = 0;
volatile uint8 t_15s_flag = 0;
volatile uint8 t_60s_flag = 0;

void open_door(void);
void change_password(void);
void change_enter_password(void);

void timerCallBack(void){
    tick++;
    if(tick == 3){
        t_15s_flag =1;
        tick = 0;
        Timer_deInit(TIMER1);
    }
    else if((Alarm_flag) && (tick == 12)){
        t_60s_flag = 1;
        tick = 0;
        Timer_deInit(TIMER1);
    }
}
void enter_password(void) {
    uint8 i,key;
    for (i = 0; i < pass_size; i++) {
    	_delay_ms(300);
        while (1) {
            key = KEYPAD_getPressedKey();
            if (key != '\0') {
                if (key == enter_button) {
                    return;
                }
                else if (key >= 0 && key <= 9) {
                	if(step==1)
                        pass1[i] = key;
                	else if(step==2)
                        pass2[i] = key;
                	else if(step==3){
                		if(wrong_pass_counter==0)
                			pass3[i] = key;
                		else if(wrong_pass_counter==1)
                			pass3_1[i] = key;
                		else if(wrong_pass_counter==2)
                			pass3_2[i] = key;
                	}
                    LCD_moveCursor(1, move+i);
                    //LCD_intgerToString(pass1[i]);
                    _delay_ms(50);
                    LCD_displayString("*");
                    break;
                }
            }
        }
    }
    while(KEYPAD_getPressedKey()!=13){
    }
}
void send_pass(uint8 password_arr[]){
	uint8 i;
	for(i=0;i<pass_size;i++){
			UART_sendByte(password_arr[i]);
			//_delay_ms(10);
		}
}
Timer_ConfigType timer = {0,39062,TIMER1,F_CPU_1024,COMPARE};
int main(void) {
    UART_ConfigType uart = {UART_8_BIT_DATA, UART_NO_PARITY, UART_ONE_STOP_BIT, 9600};

    LCD_init();
    UART_init(&uart);
    LCD_displayString("Door Lock System");
    _delay_ms(500);
    sei();

    while (1) {
        switch (step) {
            case 1:
                move = 0;
                LCD_clearScreen();
                LCD_displayString("plz enter pass:");
                LCD_moveCursor(1, 0);
                enter_password();
                send_pass(pass1);
                step++;
                break;

            case 2:
                LCD_clearScreen();
                LCD_displayString("plz re-enter the");
                LCD_moveCursor(1, 0);
                LCD_displayString("same pass:");
                move = 10;
                enter_password();
                send_pass(pass2);
                uint8 flag = UART_recieveByte();
                if (flag == 1) {
                    step++;
                } else if (flag == 0) {
                    step = 1;
                }
                break;

            case 3:
                LCD_clearScreen();
                LCD_moveCursor(0, 0);
                LCD_displayString("+ : Open Door");
                LCD_moveCursor(1, 0);
                LCD_displayString("- : Change Pass");
                uint8 choice = KEYPAD_getPressedKey();
                if (choice == '+') {
                	open_door();
                }
                else if (choice == '-') {
                	change_password();
                }
                break;
        }
    }
}
void change_password(void){
	if(c_wrong_pass_counter>2){
	    LCD_clearScreen();
	    LCD_displayString("ERROR");
	    //_delay_ms(6000);
	    c_wrong_pass_counter=0;
	}
	else{

	while(c_wrong_pass_counter<=2){
		UART_sendByte('-');
        move = 0;
        LCD_clearScreen();
        LCD_displayString("plz enter old");
        LCD_moveCursor(1, 0);
        LCD_displayString("pass:");
        LCD_moveCursor(1, 5);
        move=5;
        change_enter_password();
        send_pass(change_pass);
        uint8 c_flag =UART_recieveByte();
        if(c_flag==1){
        	c_wrong_pass_counter = 0;
        	step=1;
        	return;
        }
        else if(c_flag==0){
            c_wrong_pass_counter++;
        }

	}
    LCD_clearScreen();
    LCD_displayString("ERROR");
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    //_delay_ms(6000);


    c_wrong_pass_counter=0;
	}
}


void open_door(void){
	while(wrong_pass_counter<=2){
        UART_sendByte('+');
        move = 0;
        LCD_clearScreen();
        LCD_displayString("plz enter pass:");
        LCD_moveCursor(1, 0);
        enter_password();
        if(wrong_pass_counter==0)
        	send_pass(pass3);
        else if(wrong_pass_counter==1)
        	send_pass(pass3_1);
        else if(wrong_pass_counter==2)
        	send_pass(pass3_2);
        uint8 flag = UART_recieveByte();
        if(flag==1){
            // zbt el timer
            LCD_clearScreen();
            LCD_moveCursor(0, 3);
            LCD_displayString("Door is");
            LCD_moveCursor(1, 3);
            LCD_displayString("Unlocking");
    	    Timer_setCallBack(timerCallBack,TIMER1);
    	    Timer_init(&timer);
    	    while(t_15s_flag!=1){}
    	    t_15s_flag=0;
            LCD_clearScreen();
            if (UART_recieveByte() == 'p') {
                LCD_clearScreen();
                LCD_moveCursor(0, 0);
                LCD_displayString("Wait for people");
                LCD_moveCursor(1, 3);
                LCD_displayString("To Enter");
            }

            while (UART_recieveByte() != 'l'){}

            LCD_clearScreen();
            LCD_moveCursor(0, 3);
            LCD_displayString("Door is");
            LCD_moveCursor(1, 3);
            LCD_displayString("locking");
    	    Timer_setCallBack(timerCallBack,TIMER1);
    	    Timer_init(&timer);
    	    while(t_15s_flag!=1){}
    	    t_15s_flag=0;
            step=3;
            wrong_pass_counter=0;
            return;
        }

        else if (flag == 0) {
        	wrong_pass_counter++;
        }
	}
    LCD_clearScreen();
    LCD_displayString("ERROR");
    /*Alarm_flag=1;
    while(t_60s_flag!=1){}
    t_60s_flag=0;
    Alarm_flag=0;*/
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    Timer_setCallBack(timerCallBack,TIMER1);
    Timer_init(&timer);
    while(t_15s_flag!=1){}
    t_15s_flag=0;
    wrong_pass_counter=0;

}

void change_enter_password(void) {
    uint8 i, key;
    for (i = 0; i < pass_size; i++) {
        _delay_ms(300);
        while (1) {
            key = KEYPAD_getPressedKey();
            if (key != '\0') {
                if (key == enter_button) {
                    return;
                } else if (key >= 0 && key <= 9) {
                    change_pass[i] = key;
                    LCD_moveCursor(1, move + i);
                    _delay_ms(50);
                    LCD_displayString("*");
                    break;
                }
            }
        }
    }
    while (KEYPAD_getPressedKey() != 13) {
    }
}
