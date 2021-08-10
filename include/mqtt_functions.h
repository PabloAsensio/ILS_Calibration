#pragma once

#ifndef MQTT_FUNCTIONS_H_
#define MQTT_FUNCTIONS_H_

#include <message.h>

void on_connect(client,userdata,flags,rc);
void on_message(client, userdata, msg);
void on_publish(mqttc, obj, mid);
void on_subscribe(mqttc, obj, mid, granted_qos);

// // --------------- MQTT ------------#
// def on_connect(client,userdata,flags,rc):
//     print("Connection result: " + str(rc))
    
// def on_message(client, userdata, msg):
//     print(msg.topic+" "+str(msg.payload))

// def on_publish(mqttc, obj, mid):
//     print("JSON published") 
    
// def on_subscribe(mqttc, obj, mid, granted_qos):
//     print("Subscribed: " + str(mid) + " " + str(granted_qos))

#endif // MQTT_FUNCTIONS_H_


// mqtt::message::message	(	string_ref 	topic,
//     const void * 	payload,
//     size_t 	len,
//     int 	qos,
//     bool 	retained 
//     )	