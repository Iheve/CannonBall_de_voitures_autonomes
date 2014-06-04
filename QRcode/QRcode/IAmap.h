#pragma once
#include "IA.h"
#include <map>
class IAmap :
	public IA
{
public:
	IAmap();
	~IAmap();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
private:
	struct element {
		aruco::Marker marker;
		int lastTimeSeen;
	};
	int frame;
	map<int, struct element> elements;
	bool processing;
	bool following;
	int processing_frame_remaining = 0;
	int last_marker = -1;

};

