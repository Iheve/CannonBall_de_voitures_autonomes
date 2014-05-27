CFLAGS=-Wall -ggdb
LDFLAGS=-lmosquittopp

.PHONY: all clean

all : mqtt_sender

mqtt_sender : main.o mqtt_sender.o
	g++ $^ -o $@ ${LDFLAGS}

clean : 
	-rm -f *.o mqtt_sender
