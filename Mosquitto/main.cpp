#include "mqtt_sender.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	class mqtt_sender *sender;
	int rc;

	mosquittopp::mosquittopp::lib_init();

	sender = new mqtt_sender("sender", "localhost", 1883);
	
	while(1){
		rc = sender->loop();
		if(rc){
			sender->reconnect();
		}
        sender->send_message("Dummy message!");
        sleep(1);
	}

	mosquittopp::mosquittopp::lib_cleanup();

	return 0;
}

