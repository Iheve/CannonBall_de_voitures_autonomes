#pragma once
#include "IA.h"
class IARabbit :
	public IA
{
public:
	IARabbit();
	~IARabbit();
	virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
};

