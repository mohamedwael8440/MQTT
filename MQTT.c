/*
* MQTT.c
*
* Created: 12/23/2020 1:55:36 PM
*  Author: MahmoudH
*/
#include "MQTT.h"
/*added to git dev_branch */
/*
Assume: RemLen < 128 -> One Byte, MSB = 0
*/

static const char		name[]	= "MQTT";
static unsigned short	pck_id	= 1;

void UTFSendString(char* str){
	Uart_SendByte(0x00);
	Uart_SendByte(strlen(str));
	Uart_SendString(str);
}

void MQTT_Connect(char* id){
	unsigned char remLen = 2 + strlen(name) + 1 + 1 + 2 + 2 + strlen(id);
	Uart_SendByte(0x10);			/* type		*/
	Uart_SendByte(remLen);			/* remLen	*/
	UTFSendString(name);			/* name		*/
	Uart_SendByte(0x04);			/* level	*/
	Uart_SendByte(0x02);			/* flags	*/
	Uart_SendByte(0xFF);			/* KAT1		*/
	Uart_SendByte(0xFF);			/* KAT2		*/
	UTFSendString(id);				/* id		*/
}
void MQTT_Publish(char* topic, char* msg, char QoS){
	unsigned char remLen = 2 + strlen(topic) + 2 + strlen(msg);
	if(QoS == 0){
		Uart_SendByte(0x30);			/* type		*/
		}else{
		Uart_SendByte(0x32);			/* type		*/
	}
	Uart_SendByte(remLen);			/* remLen	*/
	UTFSendString(topic);			/* topic	*/
	if(QoS > 0){
		Uart_SendByte(pck_id>>8);		/* pck_id		*/
		Uart_SendByte(pck_id);			/* pck_id		*/
		pck_id++;
	}
	Uart_SendString(msg);
}
void MQTT_Subscribe(char* topic){
	unsigned char remLen = 2 + 2 + strlen(topic) + 1;
	Uart_SendByte(0x82);			/* type		*/
	Uart_SendByte(remLen);			/* remLen	*/
	
	Uart_SendByte(pck_id>>8);		/* pck_id		*/
	Uart_SendByte(pck_id);			/* pck_id		*/
	pck_id++;
	
	UTFSendString(topic);			/* topic	*/
	Uart_SendByte(0x01);			/* ReqQoS		*/
}