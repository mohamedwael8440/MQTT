/*
 * MQTT.h
 *
 * Created: 12/23/2020 1:55:53 PM
 *  Author: MahmoudH
 */ 


#ifndef MQTT_H_
#define MQTT_H_

#include "uart.h"
#include "string.h"

void MQTT_Connect(char* id);
void MQTT_Publish(char* topic, char* msg, char QoS);
void MQTT_Subscribe(char* topic);

#endif /* MQTT_H_ */