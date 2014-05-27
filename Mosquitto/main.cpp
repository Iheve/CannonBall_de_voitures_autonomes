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
        sender->send_message("Dummy message!");
        _sleep(1000);
	}

#ifdef __linux__ 
	MOSQPP::mosquittopp::lib_cleanup();
#elif _WIN32
	mosqpp::lib_cleanup();
#endif

	return 0;
}

