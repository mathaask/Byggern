/*
 * joystick.h
 *
 * Created: 10.10.2023 13:48:40
 *  Author: mathaask
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include "can_driver.h"
#include "ADC.h"

void joystick_value_to_node_2(ADC_readings *readings);




#endif /* JOYSTICK_H_ */