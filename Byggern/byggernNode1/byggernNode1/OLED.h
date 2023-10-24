/*
 * OLED.h
 *
 * Created: 19.09.2023 10:09:16
 *  Author: trygvemt
 */ 


#ifndef OLED_H_
#define OLED_H_

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#define RAM_BASE_ADDRESS 0x1000;

void OLED_init(void);
void OLED_reset(void);
void OLED_home(void);
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t col);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t column);
void OLED_write(char* ch);
void OLED_write_string(char* ch, uint8_t line, uint8_t col);
void OLED_print_list(char *str_list[], int hover_menu_number);

int length_of_list(const char **list);

#endif /* OLED_H_ */