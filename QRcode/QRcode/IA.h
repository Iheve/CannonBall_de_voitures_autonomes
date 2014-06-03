#pragma once
#include <vector>
#include "aruco.h"
#include"marker.h"

class IA
{
public:
	IA();
	~IA();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) = 0;
};

