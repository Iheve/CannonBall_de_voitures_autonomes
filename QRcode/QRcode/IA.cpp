#include "IA.h"


IA::IA()
{
}


IA::~IA()
{
}

void IA::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {
	*steering = 90;
	*throttle = 91;
}
