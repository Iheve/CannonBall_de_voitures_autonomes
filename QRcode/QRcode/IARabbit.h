#pragma once
#include "IA.h"
#include <map>
class IARabbit :
	public IA
{
public:
	IARabbit();
	~IARabbit();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
private:
	struct element {
		aruco::Marker marker;
		int lastTimeSeen;
	};
	int frame;
	map<int, struct element> elements;
};

