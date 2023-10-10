/*
 * interrupt.h
 *
 * Created: 12.09.2023 15:16:39
 *  Author: trygvemt
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/iom162.h>
#include "mcp2515.h"

//uint8_t button_pressed = 0;

//Funksjon som blir kalt ¨når interrupt utløses

ISR(INT0_vect);
ISR(INT2_vect);



#endif /* INTERRUPT_H_ */