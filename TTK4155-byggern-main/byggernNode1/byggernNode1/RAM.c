/*
 * RAM.c
 *
 * Created: 05.09.2023 08:59:59
 *  Author: trygvemt
 */ 

#include "RAM.h"

void xmem_init(void){
	MCUCR |= (1 << SRE);
	SFIOR |= (1 << XMM2);
	PORTC |= 0xFF; 
	
}

void xmem_write(uint8_t data, uint16_t addr){
	volatile char *ext_mem = (char *) RAM_BASE_ADDRESS;
	ext_mem[addr] = data;
}

uint8_t xmem_read(uint16_t addr){
	volatile char *ext_mem = (char *) RAM_BASE_ADDRESS;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}

void latch_test(void){
	volatile char * ext_mem = ( char *) 0x1800; 
	while (1)
	{
	for (int i = 0; i < 5; i++){
		ext_mem[i] = 1;
		_delay_ms(1000);
		//ext_mem[i] += i = 0;
	}
	
	}
}

void sram_test (void)
{
	volatile char *ext_ram = (char *) 0x1800; // s t a r t a d d r e s s f o r the sram
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf ( " starting sram test . . . \n\r") ;
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




