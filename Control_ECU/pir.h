/*
 * pir.h
 *
 *  Created on: Oct 25, 2024
 *      Author: MOH
 */

#ifndef PIR_H_
#define PIR_H_
#define PIR_PORT PORTC_ID
#define PIR_PIN  PIN2_ID
void PIR_init(void);
uint8 PIR_getState(void);

#endif /* PIR_H_ */
