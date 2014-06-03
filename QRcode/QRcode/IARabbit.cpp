#include "IARabbit.h"
#include <iostream>

using namespace aruco;

IARabbit::IARabbit()
{
}


IARabbit::~IARabbit()
{
}

void IARabbit::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {
	float factor = 0.75;
	*throttle = 91;
	for (std::vector<Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		if (it->id == 18244) {
			float x = it->getCenter().x;
			float xrel = (x - (width / 2)) / (width / 2);
			float ang = ((atan(xrel) * 180) / 3.1415) * factor + 90;
			*steering = ang;
			float d = it->Tvec.ptr<float>(0)[2];
			if (d > 2.0) {
				*throttle = 86;
			}
			else if (d > 1.0) {
				*throttle = 87;
			}
			else if (d > 0.5) {
				*throttle = 88;
			}
			cout << "a:" << ang << " d:" << d << endl;
			break;
		}
	}
}