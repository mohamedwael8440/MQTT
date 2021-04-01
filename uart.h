/*
 * uart.h
 *
 * Created: 8/17/2020 11:10:24 AM
 *  Author: MahmoudH
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU	8000000UL

#include <avr/io.h>

/* Init. uart */
void Uart_Init(unsigned long baudRate);
/* Send one Byte */
void Uart_SendByte(unsigned char data);
/* Send String (number of bytes) */
void Uart_SendString(unsigned char* str);
/* Receive one Byte (Block)*/
unsigned char Uart_ReceiveByte(void);
/* Receive one Byte (Unblock) */
unsigned char Uart_ReceiveByte_Unblock(unsigned char* dataPtr);

#endif /* UART_H_ */