/*
 * IncFile1.h
 *
 * Created: 29.08.2023 14:53:02
 *  Author: linuse
 */ 


#ifndef UARTinterface_H_
#define UARTinterface_H_

#include <avr/io.h>

void USART_Init( uint8_t ubrr );void USART_Transmit( uint8_t data );
uint8_t USART_Receive( void );






#endif /* UARTinterface */