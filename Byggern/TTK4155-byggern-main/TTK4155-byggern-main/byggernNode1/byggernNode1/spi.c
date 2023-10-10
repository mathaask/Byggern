/*
 * spi.c
 *
 * Created: 03.10.2023 09:40:07
 *  Author: trygvemt
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>



void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input, MOSI=5 SCK=7 */
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	
	PORTB |= (1<< DDB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}


uint8_t SPI_MasterTransmit(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	//printf("SPDR: %d \n\r", SPDR);
	uint8_t result = SPDR;
	return result;
}


uint8_t SPI_read(void){
/*	uint SPI_MasterTransmit(0x00);*/
	uint8_t returnData = SPI_MasterTransmit(0x00);
	return returnData;
}