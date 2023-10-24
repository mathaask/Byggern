/*
 * mcp2515.c
 *
 * Created: 03.10.2023 10:09:59
 *  Author: trygvemt
 */ 



#include "mcp2515.h"


uint8_t mcp2515_read ( uint8_t address )
{
	uint8_t result ;
	PORTB &= ~(1 << DDB4 ); // Select CAN - controller	
	
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit (address); // Send address
	result = SPI_read();	
	
	PORTB |= (1 << DDB4 ); // Deselect CAN - controller
	return result ;
	
	//return (SPI_transmit(MCP_READ, address, 0,0));
}void mcp2515_write(uint8_t address, uint8_t data){	PORTB &= ~(1 << DDB4 );	SPI_MasterTransmit(2);	SPI_MasterTransmit(address);	SPI_MasterTransmit(data);	PORTB |= (1 << DDB4 );}uint8_t mcp2515_init(void)
{
	uint8_t value ;
	SPI_MasterInit () ; // Initialize SPI
	mcp2515_reset () ; // Send reset - command
	//CANCTRL 
	// Self - test
	value = mcp2515_read(MCP_CANSTAT);
	printf("value: %d \n\r", value);
	if ((value & MODE_MASK ) != MODE_CONFIG) {
		printf (" MCP2515 is NOT in configuration mode after reset !\n");
		return 1;
	} else {
		printf (" MCP2515 is in configuration mode after reset !\n");
	}
	
	uint8_t BRP = FCPU / (2 * NUMBER_OF_TQ * BAUDRATE);
	
	
	mcp2515_write(MCP_CNF1, SJW4 | BRP-1);
	mcp2515_write(MCP_CNF2, BTLMODE | SAMPLE_3X | ((PS1-1)<<3) | (PROPAG-1));
	mcp2515_write(MCP_CNF3, WAKFIL_DISABLE | (PS2-1));
	
	value = mcp2515_read(MCP_CNF1);
	printf("CNF1: %d \n\r", value);
	value = mcp2515_read(MCP_CNF2);
	printf("CNF2: %d \n\r", value);
	value = mcp2515_read(MCP_CNF3);
	printf("CNF3: %d \n\r", value);
	
	
	
	// More initialization
	printf("value: %d \n\r", value);
	
	return 0;
}

void mcp2515_reset(void){
	PORTB &= ~(1 << DDB4 );
	SPI_MasterTransmit(MCP_RESET); 
	PORTB |= (1 << DDB4 );
}


void mcp2515_request_to_send(uint8_t buffers){
	PORTB &= ~(1 << DDB4 );
	SPI_MasterTransmit(0x80|buffers);
	PORTB |= (1 << DDB4 );
}

uint8_t mcp2515_read_status(void){
	uint8_t dataOut;
	PORTB &= ~(1 << DDB4 );
	SPI_MasterTransmit(MCP_READ_STATUS);
	dataOut = SPI_MasterTransmit(0x00);
	PORTB |= (1 << DDB4 );
	return dataOut;
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	PORTB &= ~(1 << DDB4 );
	
	SPI_MasterTransmit(5);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	
	PORTB |= (1 << DDB4 );
}

CAN_message_t mcp2515_read_rx_buffer(uint8_t buffer){
	CAN_message_t msg;
	PORTB &= ~(1 << DDB4 );
	buffer = (0x10 & buffer) << 1;
	SPI_MasterTransmit(0x10010000 & buffer);
	
	msg.id = SPI_MasterTransmit(0x00) << 3;
	msg.id = msg.id | SPI_MasterTransmit(0x00) >> 5;
	SPI_MasterTransmit(0x00);
	SPI_MasterTransmit(0x00);
	
	msg.length = SPI_MasterTransmit(0x00);
	for (uint8_t i = 0; i < msg.length;i++)
	{
		msg.data[i] = SPI_MasterTransmit(0x00);
	
	}
	printf("id: %d\n\r", msg.id);
	printf("data1: %d\n\r", msg.data[0]);
	PORTB |= (1 << DDB4 );
	return msg;
}

//Siste som skjer