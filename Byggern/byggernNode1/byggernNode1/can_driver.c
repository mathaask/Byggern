/*
 * can_driver.c
 *
 * Created: 03.10.2023 16:53:08
 *  Author: trygvemt
 */ 

#include "can_driver.h"
#include "mcp2515.h"


void can_init(){
	mcp2515_init();
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL); //NORMAL mode
	mcp2515_write(MCP_CANINTE, MCP_RX0IF|MCP_RX1IF);
	MCUCR |= (1<<ISC01);
	GICR |= (1 << INT0);
	sei();
}

void can_sendMessage(CAN_message_t *msg){
	//Assuming TX buffer 0 is used for simplicity
	// TODO: Ideally, check which TX buffer is free
	
	printf("\n\n\r");
	printf("\n\n\r");
	mcp2515_write(TXB0SIDH, (uint8_t)(msg->id >> 3));
	mcp2515_write(TXB0SIDL, (uint8_t)(msg->id << 5));
		printf("id sent: %d\n\r", msg->id);
	
	mcp2515_write(TXB0DLC, msg->length);
	printf("msg.length sent: %d\n\r", msg->length);
	for (uint8_t i=0;i<msg->length;i++){
		mcp2515_write(TXB0D0+i, msg->data[i]);
		printf("data sent: %d\n\r", msg->data[i]);
	}
	mcp2515_request_to_send(0b001);
	printf("\n\r");
}



CAN_message_t can_recieveMessage() {
	// Define a struct to store the received message data
	CAN_message_t msg;

	// Read the CANINTF register to check for received message flags
	uint8_t intf = mcp2515_read(MCP_CANINTF);
	
	// Define a variable to determine which buffer (0 or 1) has the message
	uint8_t bufferToReadFrom;

	// Check if the RX0 buffer has a new message
	if (intf & MCP_RX0IF) {
		bufferToReadFrom = 0;
		printf("RX0 buffer has a new message \n\r");
		// Clear the RX0 interrupt flag to acknowledge the received message
		mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
		
		// Check if the RX1 buffer has a new message
		} else if (intf & MCP_RX1IF) {
		bufferToReadFrom = 1;
		printf("RX1 buffer has a new message \n\r");
		// Clear the RX1 interrupt flag to acknowledge the received message
		mcp2515_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
		
		} else {
		// If neither buffer has a message, it might be an error. Return an empty message.
		memset(&msg, 0, sizeof(CAN_message_t));
		printf("Neither buffer has a message \n\r");
		return msg;
	}

	// Determine the starting register for the buffer that has the message
	uint8_t startingRegister = (bufferToReadFrom == 0) ? MCP_RXB0SIDH : MCP_RXB1SIDH;
	
	// Extract the Message ID from the SIDH and SIDL registers
	msg.id = (uint8_t)mcp2515_read(startingRegister) << 3; // Get the upper part of the ID
	msg.id |= mcp2515_read(startingRegister + 1) >> 5;      // Get the lower part of the ID
	
	printf("received id: %d\n\n\r", msg.id);
	
	// Extract the data length code (DLC) for the message. Only the lower 4 bits of DLC give the length.
	msg.length = mcp2515_read(startingRegister + 4) & 0x0F;
	
	printf("Message received\n\r");
	printf("msg.length: %d\n\r", msg.length);
	
	// Read the actual data bytes based on the determined message length
	for (uint8_t i = 0; i < msg.length; i++) {
		// Offset by 5 to reach the data section from the starting register of a buffer
		msg.data[i] = mcp2515_read(startingRegister + 5 + i);
		
		printf("data received: %d\n\r", msg.data[i]);
	}

	// Return the received message
	return msg;
}

