/*
 * app.c
 *
 *  Created on: Oct 25, 2024
 *      Author: MOH
 *
 *  */
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "std_types.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "motor.h"
#include "timer.h"
#include "twi.h"
#include "pwm.h"
#include "pir.h"
#include <avr/interrupt.h>


#define EEPROM_START_ADDRESS 0x0311
#define pass_size 5
uint8 recived_password_1[pass_size]={0};
uint8 recived_password_2[pass_size]={0};
uint8 recived_password_3[pass_size]={0};
uint8 recived_password_3_1[pass_size]={0};
uint8 recived_password_3_2[pass_size]={0};
uint8 change[pass_size]={0};
uint8 saved_password[pass_size]={0};
uint8 flag=1;
uint8 step=1;
uint8 Alarm_flag=0;
volatile uint8 tick = 0;
volatile uint8 t_15s_flag = 0;
volatile uint8 t_60s_flag = 0;
uint8 wrong_pass_counter=0;


Timer_ConfigType timer={0,39062,TIMER1,F_CPU_1024,COMPARE};
UART_ConfigType uart={UART_8_BIT_DATA,UART_NO_PARITY,UART_ONE_STOP_BIT,9600};

void open_door(void);
void change_password(void);

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
void savePasswordToEEPROM(uint16 password[]) {
    for (uint8 i = 0; i < pass_size; i++) {
        EEPROM_writeByte(EEPROM_START_ADDRESS + i, password[i]);
        _delay_ms(10);  // Small delay for EEPROM write stability
    }
}

int main(void) {
    Timer0_INIT_PWM();
    DcMotor_Init();
    PIR_init();
    Buzzer_init();

    UART_ConfigType uart = {UART_8_BIT_DATA, UART_NO_PARITY, UART_ONE_STOP_BIT, 9600};


    UART_init(&uart);
    sei();

    while (1) {
        switch (step) {
            case 1:
                for (char i = 0; i < pass_size; i++) {
                    recived_password_1[i] = UART_recieveByte();
                }
                step++;
                break;

            case 2:
                for (char i = 0; i < 5; i++) {
                    recived_password_2[i] = UART_recieveByte();
                }
                for (char i = 0; i < 5; i++) {
                    if (recived_password_1[i] != recived_password_2[i]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1) {
                    step++;
                    UART_sendByte(1);
                    EEPROM_writeData(EEPROM_START_ADDRESS,recived_password_1,pass_size);
                } else {
                    step = 1;
                    flag = 1;
                    UART_sendByte(0);
                }
                break;

            case 3:
                if (UART_recieveByte() == '+') {
                	open_door();
                	if(wrong_pass_counter>2){
                	    Buzzer_on();
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
                	    Buzzer_off();
                	    Alarm_flag=0;
                		/*Buzzer_on();
                		_delay_ms(6000);
                		Buzzer_off();*/
                		wrong_pass_counter=0;
                	}
                }
                else //(UART_recieveByte() == '-') {
                	change_password();
                //}
                break;
        }
    }
}

void open_door(void) {
    if (wrong_pass_counter > 2) {
        /*Buzzer_on();
        _delay_ms(6000);
        Buzzer_off();*/
	    Buzzer_on();
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
	    Buzzer_off();
	    Alarm_flag=0;
        wrong_pass_counter = 0;
        return;
    } else {
        while (wrong_pass_counter <= 2) {
            flag = 1;

            for (char i = 0; i < 5; i++) {
                if (wrong_pass_counter == 0)
                    recived_password_3[i] = UART_recieveByte();
                else if (wrong_pass_counter == 1)
                    recived_password_3_1[i] = UART_recieveByte();
                else if (wrong_pass_counter == 2)
                    recived_password_3_2[i] = UART_recieveByte();
            }

            EEPROM_readData(EEPROM_START_ADDRESS, saved_password, pass_size);
            saved_password[pass_size] = '\0';

            for (char i = 0; i < 5; i++) {
                if (wrong_pass_counter == 0) {
                    if (recived_password_3[i] != saved_password[i]) {
                        flag = 0;
                        break;
                    }
                } else if (wrong_pass_counter == 1) {
                    if (recived_password_3_1[i] != saved_password[i]) {
                        flag = 0;
                        break;
                    }
                } else if (wrong_pass_counter == 2) {
                    if (recived_password_3_2[i] != saved_password[i]) {
                        flag = 0;
                        break;
                    }
                }
            }

            if (flag == 1) {
                UART_sendByte(1);
                DcMotor_Rotate(CW, 100);
        	    Timer_setCallBack(timerCallBack,TIMER1);
        	    Timer_init(&timer);
        	    while(t_15s_flag!=1){}
        	    t_15s_flag=0;
                DcMotor_Rotate(STOP, 0);

                while (PIR_getState()) {
                    DcMotor_Rotate(STOP, 0);
                    UART_sendByte('p');
                }
                UART_sendByte('l');
                DcMotor_Rotate(ACW, 100);
        	    Timer_setCallBack(timerCallBack,TIMER1);
        	    Timer_init(&timer);
        	    while(t_15s_flag!=1){}
        	    t_15s_flag=0;
                DcMotor_Rotate(STOP, 0);
                step = 3;
                return;
            } else if (flag == 0) {
                UART_sendByte(0);
                ++wrong_pass_counter;
                flag = 1;
                return;
            }
        }
    }
}

/*void change_password(void){
	while(wrong_pass_counter<=2){
    	flag=1;
        for (char i = 0; i < 5; i++) {
            if (wrong_pass_counter == 0)
                recived_password_3[i] = UART_recieveByte();
            else if (wrong_pass_counter == 1)
                recived_password_3_1[i] = UART_recieveByte();
            else if (wrong_pass_counter == 2)
                recived_password_3_2[i] = UART_recieveByte();
        }
        EEPROM_readData(EEPROM_START_ADDRESS, saved_password, pass_size);
        saved_password[pass_size] = '\0';

        for (char i = 0; i < 5; i++) {
            if (wrong_pass_counter == 0) {
                if (recived_password_3[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            } else if (wrong_pass_counter == 1) {
                if (recived_password_3_1[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            } else if (wrong_pass_counter == 2) {
                if (recived_password_3_2[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            }
        }
    	uint8 i;
        for ( i = 0; i < 5; i++) {
            if (wrong_pass_counter == 0)
                recived_password_3[i] = UART_recieveByte();
            else if (wrong_pass_counter == 1)
                recived_password_3_1[i] = UART_recieveByte();
            else if (wrong_pass_counter == 2)
                recived_password_3_2[i] = UART_recieveByte();
        }

        EEPROM_readData(EEPROM_START_ADDRESS, saved_password, pass_size);
        saved_password[pass_size] = '\0';

        for (char i = 0; i < 5; i++) {
            if (wrong_pass_counter == 0) {
                if (recived_password_3[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            } else if (wrong_pass_counter == 1) {
                if (recived_password_3_1[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            } else if (wrong_pass_counter == 2) {
                if (recived_password_3_2[i] != saved_password[i]) {
                    flag = 0;
                    break;
                }
            }
        }
        flag=1;
        if (flag == 1) {
        	wrong_pass_counter = 0;
        	UART_sendByte(1);
            step = 1;
            return;
        }
        else{
        	UART_sendByte(0);
        	wrong_pass_counter++;
        	flag=1;
        }
	}
	Buzzer_on();
	_delay_ms(6000);
	Buzzer_off();
	wrong_pass_counter=0;
}*/
void change_password(void){
	if(wrong_pass_counter>2){
	    Buzzer_on();
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
	    Buzzer_off();
		wrong_pass_counter=0;
	}
	else{
	while(wrong_pass_counter<=2){
		flag=1;
		for (char i = 0; i < 5; i++){
			change[i]=UART_recieveByte();
		}
        EEPROM_readData(EEPROM_START_ADDRESS,saved_password, pass_size);
        saved_password[pass_size] = '\0';
        for (char i = 0; i < 5; i++) {
        	if(change[i]!=saved_password[i]){
        		flag=0;
        		break;
        	}
        }
        if (flag == 1) {
        	wrong_pass_counter = 0;
        	UART_sendByte(1);
            step = 1;
            return;
        }
        else{
        	UART_sendByte(0);
        	wrong_pass_counter++;
        	flag=1;
        	if(wrong_pass_counter>2){
        	    Buzzer_on();
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
        	    Buzzer_off();
        		wrong_pass_counter=0;
        	}
        	return;
        }
	}
	}



}












