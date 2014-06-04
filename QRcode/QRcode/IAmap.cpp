#include "IAmap.h"
#include <iostream>

using namespace aruco;

IAmap::IAmap() :
frame(0)
{
	elements.clear();
}


IAmap::~IAmap()
{
}

void IAmap::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {

}