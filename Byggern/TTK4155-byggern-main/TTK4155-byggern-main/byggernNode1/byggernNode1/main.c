/*
 * byggernNode1.c
 *
 * Created: 29.08.2023 08:43:04
 * Author : linuse
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "UARTinterface.h"
#include "RAM.h"
#include "ADC.h"
#include "OLED.h"
#include "interrupt.h"
#include "can_driver.h"
#include "spi.h"
#include "mcp2515.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



ADC_Calibration ADC_calibrated_values;
ADC_readings ADC_read_values; 
int hover_line = 0;
enum JoystickDirections currentJoystickDir = NEUTRAL;
enum JoystickDirections prevJoystickDir = NEUTRAL;
extern uint8_t button_pressed;


const char *main_menu[]={
	"play",
	"karrieremodus",
	"highscore",
	"settings",
	NULL
};
	
	
int main(void)
{
	
	//Init-funksjoner
	USART_Init(MYUBRR);
	printf("initializing ADC\n\r");
	adc_init();
	xmem_init();
	OLED_init();
	interrupt_init();
	can_init();
	
	ADC_calibrated_values = pos_calibration(ADC_calibrated_values);
	char* ch = "Hello World!";
	char* ch1 = (char *) 'B';
	volatile char *ext_mem1 = (char *) 0x1000;
	volatile char *ext_mem2 = (char *) 0x1200;
	//ADC_readings data;
	
	
	OLED_reset();
	OLED_print_list(main_menu, hover_line);
	
	//CAN_message_t message1;
	//message1.id = 0xA5;
	//message1.length = 2;
	//message1.data[0] = 0xA0;
	//message1.data[1] = 0xA0;
	//can_sendMessage(&message1);

	
	//printf("for: 0x%x\n\r", mcp2515_read(MCP_CANCTRL));
	//mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
	//printf("etter: 0x%x\n\r", mcp2515_read(MCP_CANCTRL));
	//sei();
	while (1) {
	
		//cli();
		ADC_read_values = adc_read(&ADC_calibrated_values);
	
		prevJoystickDir = currentJoystickDir;
		currentJoystickDir = JoystickToEnum(ADC_read_values);
	
	
		if (prevJoystickDir != currentJoystickDir & currentJoystickDir == UP & hover_line < length_of_list(main_menu)-1){
			printf("Joystick_y value: %d \n\r", ADC_read_values.joystick_y);
			hover_line ++;
			OLED_print_list(main_menu, hover_line);
		}else if (prevJoystickDir != currentJoystickDir & currentJoystickDir == DOWN & hover_line > 0)
		{
			printf("Joystick_y value: %d \n\r", ADC_read_values.joystick_y);
			hover_line --;
			OLED_print_list(main_menu, hover_line);
		}
		else{
			//do  nothing
		}
		
		
		if(button_pressed){	
			button_pressed = 0;
			switch(hover_line){
			case 0:
				printf("play");
				//play();
				break;
			case 1:
				printf("karrieremodus");
				//karrieremodus();
				break;
			case 2:
				printf("gigascore");
				//highscore();
				break;
			case 3:
				printf("settings");
				//settings();
				break;
			}
			
		}
		//sei();
	
		//if (PE0){
			////printf("NIIIIIICE");
		//}
	joystick_value_to_node_2(&ADC_read_values);
	_delay_ms(400);
	}
}

