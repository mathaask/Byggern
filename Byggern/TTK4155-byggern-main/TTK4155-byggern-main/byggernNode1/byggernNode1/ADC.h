/*
 * ADC.h
 *
 * Created: 12.09.2023 09:05:47
 *  Author: trygvemt
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#define RAM_BASE_ADDRESS 0x1000;
#define NUMBER_OF_ANALOG_VARIABLS 4

typedef enum JoystickDirections {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
	};
	
typedef struct ADC_readings {
	
	int8_t	joystick_x;
	int8_t	joystick_y;
	uint8_t	slider_left;
	uint8_t	slider_right;
	
	} ADC_readings;
	
typedef struct ADC_Calibration {
	
	uint8_t	joystick_x;
	uint8_t	joystick_y;
	
} ADC_Calibration;
	
void adc_init(void);

ADC_readings adc_read(ADC_Calibration* calibration_values);

ADC_Calibration pos_calibration(ADC_Calibration readings);

//pos_t pos_read(void);

void adc_test(void);

#endif /* ADC_H_ */

enum JoystickDirections JoystickToEnum(ADC_readings readings);