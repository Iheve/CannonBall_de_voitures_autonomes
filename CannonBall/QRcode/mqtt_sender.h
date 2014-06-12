#ifndef MQTT_SENDER_H
#define MQTT_SENDER_H

#include <mosquittopp.h>
#include <stdint.h>

#define TOPIC_STEER "metrics/steering"
#define TOPIC_THROT "metrics/throttle"
#define TOPIC_LAPS "metrics/laps"
#define TOPIC_AVG "metrics/avg"
#define TOPIC_MODE "metrics/mode"
#define TOPIC_ACCELEROMETER "metrics/accelerometer"
#define TOPIC_NB_MARKERS "metrics/nb_markers"
#define TOPIC_CLOSEST "metrics/closest"

#if defined(__linux__)
#define MOSQPP mosquittopp
#elif defined(_WIN32) || defined(__APPLE__)
#define MOSQPP mosqpp
#endif

/**
 * Class which allow to send message throw MQTT protocol
 * @author Thibaut Coutelou, Benjamin Mugnier, Guillaume Perrin
 */
class mqtt_sender : public MOSQPP::mosquittopp
{
    public:
        /**
         * Constructor, create a connection
         * @param id the sender id
         * @param host address of the MQTT server
         * @param port port used by the MQTT server
         */
        mqtt_sender(const char *id, const char *host, int port);
        ~mqtt_sender();

        /**
         * Send message to the MQTT server, if connected
         * @param topic the topic in which the message will be send
         * @param msg message to send
         */
        void publish_to_mqtt(char *topic, char *msg);

   private:
        bool connected;
};

#endif
