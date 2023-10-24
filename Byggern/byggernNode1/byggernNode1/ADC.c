/*
 * ADC.c
 *
 * Created: 12.09.2023 09:05:20
 *  Author: trygvemt
 */ 
#include "ADC.h"
#include "RAM.h"

	
void adc_init(void){
	
	TCNT1=0;
	OCR1A = 1;
	TCCR1A |= (1<<COM1A0);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	DDRD |= (1 << PD5);	
	//DDRE |= (1 << PE0); 
	};



ADC_readings adc_read(ADC_Calibration* calibration_values){

	volatile char *ext_mem = (char *) 0x1400;

	ADC_readings readings;
	
	ext_mem[0] = 0;
	//dersom det oppstår problem, legg inn en delay for BUSY
	uint8_t temp = ext_mem[0];

	if(temp <= calibration_values->joystick_y){
		readings.joystick_y = (int8_t)((float)temp/calibration_values->joystick_y*100)-100;
		readings.joystick_y = readings.joystick_y * -1;
	} else {
		readings.joystick_y = (int8_t)((float)(temp - calibration_values->joystick_y)/(255-calibration_values->joystick_y)*100);
		readings.joystick_y = readings.joystick_y * -1;
	}
	temp = ext_mem[0];
	if(temp <= calibration_values->joystick_x){
		readings.joystick_x = (int8_t)((float)temp/calibration_values->joystick_x*100)-100;
		} 
	else {
		readings.joystick_x = (int8_t)((float)(temp - calibration_values->joystick_x)/(255-calibration_values->joystick_x)*100);
	}
	
	readings.slider_left = ext_mem[0];
	readings.slider_right = ext_mem[0];
	
	return readings;
	
	
	// VI HAR UINT I STEDET FOR INT. KAN IKKE LESE NEGATIV PROSENT. MÅ ENDRES med casting.
	
	
}


ADC_Calibration pos_calibration(ADC_Calibration readings) {
	volatile char *ext_mem = (char *) 0x1400;
	ext_mem[0] = 0;
	
	readings.joystick_y = ext_mem[0];
	readings.joystick_x = ext_mem[0];
	return readings;
}
	




void adc_test(void) {
	
		volatile char *ext_ram = (char *) 0x1400; // s t a r t a d d r e s s f o r the sram
		uint16_t ext_ram_size = 0x400;
		uint16_t write_errors = 0;
		uint16_t retrieval_errors = 0;
		printf ( " starting adc test . . . \n\r") ;
		// rand ( ) s t o r e s some i n t e r n a l s t a t e , s o c a l l i n g t h i s f u n c ti o n i n a l o o p ?- w i l l
		// y i e l d d i f f e r e n t s e e d s each time ( u n l e s s s r and ( ) i s c a l l e d b e f o r e t h i s ?- f u n c ti o n )
		uint16_t seed = rand();
		// write phase : immediately check t h a t the c o r r e c t v al u e was s t o r e d
		srand(seed);
		for (uint16_t i = 0; i < ext_ram_size ; i++) {
			uint8_t some_value = rand();
			ext_ram[i] = some_value;
			uint8_t retreived_value = ext_ram[i];
			if (retreived_value != some_value) {
				//printf( "write phase error : ext ram [%4d] = %02x ( should be %02x) \n?-" , i , retreived_value , some_value ) ;
				write_errors++;
			}
		}
		// r e t r i e v a l phase : check t h a t no v al u e s were changed du rin g o r a f t e r the?-write phase
		srand (seed);
		// r e s e t the prng t o the s t a t e i t had b e f o r e the w ri t e phase
		for(uint16_t i = 0 ; i < ext_ram_size ; i++) {
			uint8_t some_value = rand();
			uint8_t retreived_value = ext_ram[i];
			if (retreived_value != some_value) {
				//printf("retrieval phase error : ext ram [%4d ] = %02x (should be %02?-x) \n" , i , retreived_value , some_value );
				retrieval_errors++;
			}
		}
		printf("error test completed with \n\r%4d errors in write phase and \n\r%4d ?- errors in retrieval phase \n\n\r"
		, write_errors , retrieval_errors );
	}


enum JoystickDirections JoystickToEnum(ADC_readings readings){
	
	enum JoystickDirections highest_direction = NEUTRAL;
	if (abs(readings.joystick_y) > 75)
	{
		if (readings.joystick_y > 0){
			highest_direction = UP;
			} else{
			highest_direction = DOWN;
		}
	}
	if (abs(readings.joystick_x) > 75 & abs(readings.joystick_x) > abs(readings.joystick_y))
	{
		if (readings.joystick_x < 0){
			highest_direction = LEFT;
			} else{
			highest_direction = RIGHT;
		}
	}
	return highest_direction;
}
	