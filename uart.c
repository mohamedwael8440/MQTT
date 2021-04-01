/*
 * uart.c
 *
 * Created: 8/17/2020 11:10:10 AM
 *  Author: MahmoudH
 */ 
#include "uart.h"

/* Init. uart */
void Uart_Init(unsigned long baudRate){
	unsigned short baud = 0;
	baud = ((F_CPU)/(16*baudRate)) - 1;
	/* Set baud rate */
//	baud = 51;
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}
/* Send one Byte */
void Uart_SendByte(unsigned char data){
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}
/* Send String (number of bytes) */
void Uart_SendString(unsigned char* str){
	unsigned long ind = 0;
	while(str[ind] != 0){
		Uart_SendByte(str[ind]);
		ind++;
	}
}
/* Receive one Byte (Syncro. Function)*/
unsigned char Uart_ReceiveByte(void){
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) ); /* Blocking Condition */
	/* Get and return received data from buffer */
	return UDR;
}
/* Receive one Byte (Unblock) */
unsigned char Uart_ReceiveByte_Unblock(unsigned char* dataPtr){
	unsigned char status = 0;
	if (UCSRA & (1<<RXC)){
		// upload it
		(*dataPtr) = UDR;
		// update status flag
		status = 1;
	}
	else{
		status = 0;
	}
	return status;
}