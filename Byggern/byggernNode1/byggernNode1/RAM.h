/*
 * RAM.h
 *
 * Created: 05.09.2023 09:00:35
 *  Author: trygvemt
 */ 


#ifndef RAM_H_
#define RAM_H_

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#define RAM_BASE_ADDRESS 0x1000;


void xmem_init(void);
void xmem_write(uint8_t data, uint16_t addr);
uint8_t xmem_read(uint16_t addr);
void sram_test (void);
void latch_test(void);




#endif /* RAM_H_ */