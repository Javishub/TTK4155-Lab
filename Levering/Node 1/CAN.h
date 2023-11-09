#include "mcp2515.h"

static void interrupt_can_recieve_init() {
	// set INT0 as input
	DDRD &= ~(1 << PD2);

	// enable INT0 interrupt vector
	GICR |= (1 << INT0);

	// interrupt on falling edge
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
}


void can_init(){
	mcp_init();
	//enable interrupt on both rx1 and rx0
	mcp_bit_modify(MCP_CANINTE, MCP_RX1IF | MCP_RX0IF, 0xFF);
	interrupt_can_recieve_init();
}

typedef struct Message{
	unsigned int id;
	uint8_t length;
	char data[8];
	}message_t, *message_ptr;
	
	
	
void can_send(message_ptr message){
	mcp_write(MCP_TXB0SIDH, message->id/8);
	mcp_write(MCP_TXB0SIDL, (message->id%8) << 5);
	
	mcp_write(MCP_TXB0DLC, message->length);
	
	for(int i = 0; i<message->length;i++){
		mcp_write(MCP_TXB0D0 + i, message->data[i]);
	}
	
	mcp_request_to_send(0);
}

message_t can_receive(){
	message_t message = {};
		if (mcp_read(MCP_CANINTF) & MCP_RX0IF){
			
			uint8_t id_low = mcp_read(MCP_RXB0SIDL)/0b100000;
			uint8_t id_high = mcp_read(MCP_RXB0SIDH);
			message.id = id_high * 0b1000 + id_low;
			
			message.length = mcp_read(MCP_RXB0DLC);
			if(message.length > 7){
				message.length = 7;
			}
			
			for (int i = 0; i < message.length; i++)
			{
				mcp_bit_modify(MCP_CANINTF, 1<<1, 0);
				message.data[i] = mcp_read(MCP_RXB0D0 + i);
			}
			
			return message;
		}
		else if (mcp_read(MCP_CANINTF) & MCP_RX1IF){
			
			uint8_t id_low = mcp_read(MCP_RXB1SIDL)/0b100000;
			uint8_t id_high = mcp_read(MCP_RXB1SIDH);
			message.id = id_high * 0b1000 + id_low;
			
			message.length = mcp_read(MCP_RXB1DLC);
			if(message.length > 7){
				message.length = 7;
			}
			
			for (int i = 0; i < message.length; i++)
			{
				message.data[i] = mcp_read(MCP_RXB1D0 + i);
			}
			
			mcp_bit_modify(MCP_CANINTF, 1<<1, 0);
			return message;
		}
		
	
	
}