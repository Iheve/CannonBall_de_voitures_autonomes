#include <cstdio>
#include <cstring>
#include <iostream>

#include <stdint.h>
#include "mqtt_sender.h"

mqtt_sender::mqtt_sender(const char *id, const char *host, int port) : mosquittopp(id)
{
    int keepalive = 60;
    bool clean_session = true;

    /* Connect immediately. This could also be done by calling
     * mqtt_sender->connect(). */
    connect(host, port, keepalive);
};

void mqtt_sender::send_message(char *topic, char *msg) {
    std::cout << "Sending message.." << std::endl;
    publish(NULL, topic, strlen(msg)*sizeof(uint8_t), (uint8_t *) msg); //Client
    std::cout << "Sent!" << std::endl;
}
