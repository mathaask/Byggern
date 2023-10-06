/*
 * OLED.c
 *
 * Created: 19.09.2023 10:09:05
 *  Author: trygvemt
 */ 

#include "UARTinterface.h"

#include "OLED.h"
#include "font.h"



void OLED_init(void){
	volatile char *ext_mem = (char *) 0x1000;
	printf("initializing OLED\n\r");
	ext_mem[0] = (0xae); // display off
	ext_mem[0] = (0xa1); //segment remap
	ext_mem[0] = (0xda); //common pads hardware: alternative
	ext_mem[0] = (0x12);
	ext_mem[0] = (0xc8); //common output scan direction:com63~com0
	ext_mem[0] = (0xa8); //multiplex ration mode:63
	ext_mem[0] = (0x3f);
	ext_mem[0] = (0xd5); //display divide ratio/osc. freq. mode
	ext_mem[0] = (0x80);
	ext_mem[0] = (0x81); //contrast control
	ext_mem[0] = (0x50);
	ext_mem[0] = (0xd9); //set pre-charge period
	ext_mem[0] = (0x21);
	ext_mem[0] = (0x20); //Set Memory Addressing Mode,
	ext_mem[0] = (0x02);
	ext_mem[0] = (0xdb); //VCOM deselect level mode
	ext_mem[0] = (0x30);
	ext_mem[0] = (0xad); //master configuration
	ext_mem[0] = (0x00);
	ext_mem[0] = (0xa4); //out follows RAM content
	ext_mem[0] = (0xa6); //set normal display
	ext_mem[0] = (0xaf); // display on
}

void OLED_reset(void){
	for (uint8_t i=0;i<8;i++)
	{
		OLED_clear_line(i);
	}
	OLED_goto_line(0);
	OLED_goto_column(0);
}
void OLED_home(void);
void OLED_goto_line(uint8_t line){
		volatile char *ext_mem_cmd = (char *) 0x1000;
		ext_mem_cmd[0] = 0b10110000 | line;
}
void OLED_goto_column(uint8_t col){
	volatile char *ext_mem_cmd = (char *) 0x1000;
	ext_mem_cmd[0] = col & 0b00001111;
	ext_mem_cmd[0] = 0x10 | (col >> 4);
	/*ext_mem_cmd[0] = 0x21;
	ext_mem_cmd[0] = col;
	ext_mem_cmd[0] = 127;*/
	
}
void OLED_clear_line(uint8_t line){
	OLED_goto_line(line);
	OLED_goto_column(0);
	volatile char *ext_mem = (char *) 0x1200;
	for (uint8_t i=0;i<128;i++)
	{
		ext_mem[0] = 0;
	}
}
void OLED_pos(uint8_t row, uint8_t column){
	OLED_goto_line(row);
	OLED_goto_column(column);
}


void OLED_write(char* ch){	
	volatile char *ext_mem1 = (char *) 0x1200;
	
	for (uint8_t i=0;i<8;i++)
	{
		ext_mem1[0] = pgm_read_byte(&(font8[(uint8_t)ch-32][i]));	
		
	}

}

void OLED_write_string(char* ch, uint8_t line, uint8_t col){
	OLED_pos(line, col);
	size_t len = strlen(ch);
	for(size_t i = 0; i < len; i++){

		OLED_write(ch[i]);
	}
}

void OLED_print_list(char *str_list[], int hover_menu_number){
	//size_t len = sizeof(str_list);
	OLED_reset();
	int length = length_of_list(str_list);
	printf("length of list %d \n\r", length);
	for (size_t i = 0; i < length; i++){
		
		if (i == hover_menu_number)
		{
			OLED_goto_column(0);
			OLED_goto_line(i);				
			OLED_write(127);
			OLED_write_string(str_list[i],i,10);
		} 
		else
		{
			OLED_write_string(str_list[i],i,10);
		}		
	}
}


int length_of_list(const char **list){
	int number_of_strings = 0;
	while(list[number_of_strings] != NULL){
		number_of_strings++;
	}
	return number_of_strings;
	
}
