/*
 * GccApplication2.c
 *
 * Created: 04.09.2023 09:07:11
 * Author : Ludvi
 */ 

#include <avr/io.h>
#include <string.h>
#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}
void UART_TransmitString(const char *str) {
	// Iterate through the string and send each character
	for (size_t i = 0; i < strlen(str); i++) {
		USART_Transmit(str[i]);
	}
}
int main() {
	UART_Init(MYUBRR);

	while (1) {
		// Sending a string
		const char *message = "Hello, World!\n";
		UART_TransmitString(message);

		// Receiving data
		char receivedChar = USART_Receive();
		USART_Transmit(receivedChar); // Echo received character
	}

	return 0;
}