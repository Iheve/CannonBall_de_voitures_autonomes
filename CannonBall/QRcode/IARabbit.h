#pragma once
#include "IA.h"
#include <map>


/**
 * This artificial intelligence allow the car to follow a marker : the Rabbit.
 * The ID of the marker to follow is passed as argument.
 */
class IARabbit :
	public IA
{
public:
	IARabbit(int argc, char *argv[]);
	~IARabbit();
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

