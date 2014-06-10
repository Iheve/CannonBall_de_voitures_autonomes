#include "IA.h"


IA::IA()
{
}


IA::~IA()
{
}

void IA::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {
	cout << "WARNING: you called the empty IA which will just stop the motors" << endl;
	*steering = 90;
	*throttle = 91;
}
