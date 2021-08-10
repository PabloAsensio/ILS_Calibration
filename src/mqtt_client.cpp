#include "mqtt_client.h"

#define PUBLISH_TOPIC "EXAMPLE_TOPIC"

#if DEBUG
#include <iostream>
#endif

std::vector<std::future<void>> mqtt_Futures;
volatile sig_atomic_t MANUAL_STOP_MQTT;
void inthand_mqtt(int signum)
{
    MANUAL_STOP_MQTT = 1;
    KILL_PROGRAM = true;
    STOP_MQTT = true;
}

mqtt_client::mqtt_client(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    int keepalive = DEFAULT_KEEP_ALIVE;
    connect(host, port, keepalive);
}

mqtt_client::~mqtt_client()
{
}

void mqtt_client::on_connect(int rc)
{
    if (!rc)
    {
#if DEBUG
        std::cerr << "Connected - code " << rc << std::endl;
#endif
    }
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
#if DEBUG
    std::cerr << "Subscription succeeded." << std::endl;
#endif
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
    int payload_size = MAX_PAYLOAD + 1;
    char buf[payload_size];

    if (!strcmp(message->topic, "EXAMPLE_TOPIC"))
    {
        memset(buf, 0, payload_size * sizeof(char));

        /* Copy N-1 bytes to ensure always 0 terminated. */
        memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));

        if (!strcmp(buf, "ON"))
        {
            snprintf(buf, payload_size, "Turning on...");
            // std::cerr << "Turning on..." << '\n';
            STOP_MQTT = false;
        }

        if (!strcmp(buf, "OFF"))
        {
            snprintf(buf, payload_size, "Turning off...");
            // std::cerr << "Turning off..." << '\n';
            STOP_MQTT = true;
            KILL_PROGRAM = true;
            disconnect();
        }
    }
}

void read_topic(const char *host, int port)
{
    class mqtt_client *reciver_client;
    reciver_client = new mqtt_client("reciver_client", host, port);

    const char *reciver_username = "receivedrone";
    const char *reciver_password = "Rec3iv3Dr0n3";
    reciver_client->username_pw_set(reciver_username, reciver_password);
    int reciver_connected = reciver_client->connect_async(host, port, 60);
    if (reciver_connected == 0)
        std::cerr << "reciver_client"
                  << " connected to: " << host << ":" << port << '\n';
    int reciver_subscibed = reciver_client->subscribe(NULL, MQTT_TOPIC, 2);
    if (reciver_subscibed == 0)
        std::cerr << "Reciber subscibed to: " << MQTT_TOPIC << '\n';
    reciver_client->loop_forever();
    std::cerr << "Desconnected to: " << host << ":" << port << '\n';
}

void listen_on_topic(mqtt_client *reciver_client, const char *topic)
{
    PROFILE_FUNCTION();
    int reciver_subscibed = reciver_client->subscribe(NULL, topic, 2);
    if (reciver_subscibed == 0)
        std::cerr << "Reciber subscibed to: " << topic << '\n';
    else
        std::cerr << "Reciber subscibed to: NINGUNO " << '\n';

    reciver_client->loop_start();

    while (!KILL_PROGRAM)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (KILL_PROGRAM)
    {
        std::cerr << "OFF RECIVED\n";
        reciver_client->disconnect();
        reciver_client->loop_stop();
    }
}

void publish_on_topic(mqtt_client *publisher_client, const char *topic)
{
    std::string input;
    while (!MANUAL_STOP_MQTT)
    {
        std::getline(std::cin, input);
        char *msg = new char[input.length() + 1];
        std::strcpy(msg, input.c_str());
        if (!MANUAL_STOP_MQTT)
            publisher_client->publish(NULL, topic, strlen(msg), msg, 2);
        else
        {
            publisher_client->disconnect();
            std::cerr << "Publisher disconnected to: " << BROKER_ADDRESS << "\n";
        }
    }
}

int main_mosquitto(int argc, char *argv[])
{
    // signal(SIGINT, inthand_mqtt);
    class mqtt_client *reciver_client;
    class mqtt_client *publisher_client;
    int rc;

    char client_id[] = CLIENT_ID;
    char host[] = BROKER_ADDRESS;
    int port = MQTT_PORT;

    mosqpp::lib_init();

    reciver_client = new mqtt_client("reciver_client", host, port);
    publisher_client = new mqtt_client("publisher_client", host, port);

    const char *reciver_username = "receivedrone";
    const char *reciver_password = "Rec3iv3Dr0n3";
    reciver_client->username_pw_set(reciver_username, reciver_password);

    const char *publisher_username = "sendrone";
    const char *publisher_password = "S3nDr0n3";
    publisher_client->username_pw_set(publisher_username, publisher_password);

    int reciver_connected = reciver_client->connect_async(host, port, 60);
    int publisher_connected = publisher_client->connect(host, port, 60);

    if (reciver_connected == 0)
        std::cerr << "reciver_client"
                  << " connected to: " << host << ":" << port << '\n';

    if (publisher_connected == 0)
        std::cerr << "publisher_client"
                  << " connected to: " << host << ":" << port << '\n';

    mqtt_Futures.push_back(std::async(std::launch::async, publish_on_topic, publisher_client, MQTT_TOPIC));

    int reciver_subscibed = reciver_client->subscribe(NULL, MQTT_TOPIC, 2);
    if (reciver_subscibed == 0)
        std::cerr << "Reciber subscibed to: " << MQTT_TOPIC << '\n';

    reciver_client->loop_forever();

    std::cerr << "Desconnected to: " << host << ":" << port << '\n';

    mosqpp::lib_cleanup();

    return 0;
}
