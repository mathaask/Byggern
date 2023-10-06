/*
 * can_driver.h
 *
 * Created: 03.10.2023 16:53:28
 *  Author: trygvemt
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"


typedef struct CAN_message_t{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
	
	} CAN_message_t;
	
#include "mcp2515.h"

void can_init(void);

void can_sendMessage(CAN_message_t *msg);

CAN_message_t can_recieveMessage(void);



#endif /* CAN_DRIVER_H_ */