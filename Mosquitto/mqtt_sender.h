#ifndef MQTT_SENDER_H
#define MQTT_SENDER_H

#include <mosquittopp.h>
#include <stdint.h>

#ifdef __linux__ 
#define MOSQPP mosquittopp
#elif _WIN32
#define MOSQPP mosqpp
#else
#endif

class mqtt_sender : public MOSQPP::mosquittopp
{
	public:
		mqtt_sender(const char *id, const char *host, int port);
		~mqtt_sender();

        void send_message(char *msg);

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos);
};

#endif
