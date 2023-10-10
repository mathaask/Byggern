/*
 * spi.h
 *
 * Created: 03.10.2023 09:39:56
 *  Author: trygvemt
 */ 


#ifndef SPI_H_
#define SPI_H_


void SPI_MasterInit(void);

uint8_t SPI_MasterTransmit(uint8_t cData);

uint8_t SPI_read(void);

char SPI_transmit(char command, char address, char* data, int data_lenght);

#endif /* SPI_H_ */