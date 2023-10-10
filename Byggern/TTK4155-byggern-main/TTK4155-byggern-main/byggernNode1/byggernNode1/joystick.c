/*
 * joystick.c
 *
 * Created: 10.10.2023 13:48:24
 *  Author: mathaask
 */ 

#include "joystick.h"

void joystick_value_to_node_2(ADC_readings *readings){
	CAN_message_t msg;
	printf("joystick: %d\n\r", readings->joystick_x);
	enum JoystickDirections pos = JoystickToEnum(*readings);
	if (pos != NEUTRAL){
		msg.id = 0;
		msg.length = 1;
		msg.data[0] = readings->joystick_x;
		can_sendMessage(&msg);
	}
}