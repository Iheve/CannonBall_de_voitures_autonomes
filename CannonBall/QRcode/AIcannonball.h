#pragma once
#include "AI.h"
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
class AICannonball :
	public AI
{
public:
	AICannonball(int argc, char *argv[]);
	~AICannonball();
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

