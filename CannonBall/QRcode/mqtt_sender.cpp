#include <cstdio>
#include <cstring>
#include <iostream>

#include <stdint.h>
#include "mqtt_sender.h"

mqtt_sender::mqtt_sender(const char *id, const char *host, int port) : mosquittopp(id)
{
    int keepalive = 60;
    connected = false;

    /* Connect immediately. This could also be done by calling
     * mqtt_sender->connect(). */
    connected = connect_async(host, port, keepalive);
    loop_start();
};

void mqtt_sender::publish_to_mqtt(char *topic, char *msg) {
    if (connected)
        publish(NULL, topic, strlen(msg)*sizeof(uint8_t), (uint8_t *) msg); //Client
}
