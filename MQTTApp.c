/*
* MQTTApp.c
*
* Created: 12/23/2020 1:08:21 PM
*  Author: MahmoudH
*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define			F_CPU	8000000UL
#include		"util/delay.h"
#include		"MQTT.h"

void T_Connect(void* pvData);
void T_RespHandle(void* pvData);
void T_Control(void* pvData);
void T_Monitor(void* pvData);
void system_init(void);

unsigned long temp = 20;

int main(void)
{
	system_init();

	//xTaskCreate(T_Monitor,NULL,100,NULL,1,NULL);
	//xTaskCreate(T_Control,NULL,100,NULL,2,NULL);
	xTaskCreate(T_Connect,NULL,100,NULL,3,NULL);
	xTaskCreate(T_RespHandle,NULL,100,NULL,4,NULL);
	
	vTaskStartScheduler();
}

void T_Connect(void* pvData){
	MQTT_Connect("IDMHAA1389");
	vTaskDelay(1000);
	MQTT_Subscribe("control");
	vTaskDelay(1000);
	while(1){
		vTaskDelay(5000);
	}
}
typedef enum{
	RXS_NONE, RXS_CONNACK, RXS_SUBACK, RXS_PUBACK, RXS_PUB
}RX_State;
void T_RespHandle(void* pvData){
	unsigned char data = 0;
	unsigned char remLen = 0;
	RX_State rx_state = RXS_NONE;
	unsigned char rxdBytes = 0;
	while(1){
		if (Uart_ReceiveByte_Unblock(&data))
		{
			switch (rx_state)
			{
				case RXS_NONE:
				if (data == 0x20)
				{
					rx_state = RXS_CONNACK;
					rxdBytes = 1;
				}
				else if (data == 0x90){
					rx_state = RXS_SUBACK;
					rxdBytes = 1;
				}
				else if (data == 0x40){
					rx_state = RXS_PUBACK;
					rxdBytes = 1;
				}
				else if (data == 0x30){
					rx_state = RXS_PUB;
					rxdBytes = 1;
				}
				break;
				case RXS_CONNACK:
				rxdBytes++;
				if (rxdBytes == 4) //return code  
				{
					/* CONACK received */
					rx_state = RXS_NONE;
				}
				break;
				case RXS_SUBACK:
				rxdBytes++;
				if (rxdBytes == 5)
				{
					/* SUBACK received */
					rx_state = RXS_NONE;
				}
				break;
				case RXS_PUBACK:
				if (rxdBytes == 4)
				{
					/* PUBACK received */
					rx_state = RXS_NONE;
				}
				break;
				case RXS_PUB:
				
				if (rxdBytes == 1)
				{
					remLen = data;
				}
				if(rxdBytes == (remLen + 1)){
					if(data == '1'){
						PORTA = 0xFF;
					}
					else if(data == '0'){
						PORTA = 0x00;
					}
					/* PUB received */
					rx_state = RXS_NONE;
				}
				rxdBytes++;
				break;
			}
		}
		
		else {
			vTaskDelay(1);
		}
	}
}
void T_Control(void* pvData){
	while(1){
		vTaskDelay(5000);
	}
}
char buffer[50];
void T_Monitor(void* pvData){
	while(1){
		sprintf(buffer,"mah_hus:%d",temp++);
		MQTT_Publish("nti_temp",buffer,1);
		vTaskDelay(5000);
	}
}
void system_init(void){
	Uart_Init(9600);
	//Uart_SendString("Started...\n");
	DDRA	= 0xFF;
	PORTA	= 0x00;
}