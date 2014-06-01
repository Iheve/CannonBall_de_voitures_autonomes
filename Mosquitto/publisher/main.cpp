#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include "mqtt_sender.h"

int main(int argc, char *argv[])
{
	class mqtt_sender *sender;
	int rc;

#ifdef __linux__
	MOSQPP::mosquittopp::lib_init();
#elif _WIN32
	mosqpp::lib_init();
#endif

	sender = new mqtt_sender("sender", "localhost", 1883);
	
	while(1){
		rc = sender->loop();
		if(rc){
			sender->reconnect();
		}
        sender->send_message("presence", "Hello mqtt");
#if defined(__linux__) || defined(__APPLE__)
        sleep(1);
#elif defined(_WIN32)
        Sleep(1000);
#endif
	}

#ifdef __linux__
	MOSQPP::mosquittopp::lib_cleanup();
#elif _WIN32
	mosqpp::lib_cleanup();
#endif

	return 0;
}

