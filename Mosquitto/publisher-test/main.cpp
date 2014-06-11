#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif
#include <stdio.h>

#include "mqtt_sender.h"

/* For test propose only !! */
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
    int i = 0;
	
	while(1){
		rc = sender->loop();
		if(rc){
			sender->reconnect();
		}
        char b[50], c[50];
        sprintf(b, "%d", rand() % 180);
        sprintf(c, "%f:%f:%f", (float)(rand()%180)/100.f, (float)(rand()%180)/100.f, (float)(rand()%180)/100.f);
        sender->send_message("metrics/steering", b);
        sender->send_message("metrics/throttle", b);
        sender->send_message("metrics/avg", b);
        sender->send_message("metrics/accelerometer", c);
        sender->send_message("metrics/mode", "Rabbit");
#if defined(__linux__) || defined(__APPLE__)
        sleep(1);
#elif defined(_WIN32)
        Sleep(1000);
#endif
        i++;
        if (i >= 180)
            i = 0;
	}

#ifdef __linux__
	MOSQPP::mosquittopp::lib_cleanup();
#elif _WIN32
	mosqpp::lib_cleanup();
#endif

	return 0;
}

