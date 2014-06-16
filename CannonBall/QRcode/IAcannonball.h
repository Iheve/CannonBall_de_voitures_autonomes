#pragma once
#include "IA.h"
#include <map>
#include <stdbool.h>

struct door {
	int left;
	int right;
	struct door* next;
};

/**
 * This artificial intelligence alows the car to follow a circuit.
 * The doors of the circuit are markers. The markers order is given in a file :
 * left1 right1 left2 right2...
 */
class IAcannonball :
	public IA
{
public:
	IAcannonball(int argc, char *argv[]);
	~IAcannonball();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
private:
	struct element {
		aruco::Marker marker;
		int lastTimeSeen;
		bool valid;
	};
	int frame;
	int idle;
	map<int, struct element> elements;
	struct door* currentDoor;
};

