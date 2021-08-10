#pragma once

#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include "global.h"

#include <mosquittopp.h>
#include <cstring>
#include <cstdio>
#include <vector>

// To stop correctly by hand
#include <unistd.h>
#include <signal.h>

// Multiprocesing
#include <future>

#include <iterator>

#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

#define CLIENT_ID "Client_ID"
// #define BROKER_ADDRESS "localhost"
#define BROKER_ADDRESS "193.201.107.70"
#define MQTT_PORT 1883;
#define MQTT_TOPIC "EXAMPLE_TOPIC"
#define DEBUG 1

class mqtt_client : public mosqpp::mosquittopp
{
public:
    mqtt_client(const char *id, const char *host, int port);
    ~mqtt_client();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

void listen_on_topic(mqtt_client *publisher_client, const char *topic);
void read_topic(const char *host, int port);
int main_mosquitto(int argc, char *argv[]);

#endif // MQTT_CLIENT_H_>