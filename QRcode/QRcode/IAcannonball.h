#pragma once
#include "IA.h"
#include <map>

struct door {
	int left;
	int right;
	struct door* next;
};

class IAcannonball :
	public IA
{
public:
	IAcannonball();
	~IAcannonball();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
private:
	struct element {
		aruco::Marker marker;
		int lastTimeSeen;
	};
	int frame;
	int idle;
	map<int, struct element> elements;
	struct door* currentDoor;
};

