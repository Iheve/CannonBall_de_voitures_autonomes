#ifndef MQTT_SENDER_H
#define MQTT_SENDER_H

#include <mosquittopp.h>
#include <stdint.h>

#if defined(__linux__)
#define MOSQPP mosquittopp
#elif defined(_WIN32) || defined(__APPLE__)
#define MOSQPP mosqpp
#endif

class mqtt_sender : public MOSQPP::mosquittopp
{
    public:
        mqtt_sender(const char *id, const char *host, int port);
        ~mqtt_sender();

        void send_message(char *topic, char *msg);
};

#endif
