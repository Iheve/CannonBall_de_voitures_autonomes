#pragma once
#include "AI.h"
#include <map>


/**
 * This artificial intelligence allow the car to follow a marker : the Rabbit.
 * The ID of the marker to follow is passed as argument.
 */
class AIRabbit :
	public AI
{
public:
	AIRabbit(int argc, char *argv[]);
	~AIRabbit();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
private:
	struct element {
		aruco::Marker marker;
		int lastTimeSeen;
	};
	int frame;
	int target;
	map<int, struct element> elements;
};

