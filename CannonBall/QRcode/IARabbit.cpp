#include "IARabbit.h"
#include <iostream>

using namespace aruco;

IARabbit::IARabbit() :
frame(0)
{
	elements.clear();
}


IARabbit::~IARabbit()
{
}

void IARabbit::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {

	//Update our map
	frame++;
	for (std::vector<Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		elements[it->id].marker = *it;
		elements[it->id].lastTimeSeen = frame;
	}

	struct element rabbit = elements[18244];
	float x = rabbit.marker.getCenter().x;
	float d = rabbit.marker.Tvec.ptr<float>(0)[2];

	if (rabbit.lastTimeSeen == 0) {
		*steering = 90;
		*throttle = 91;
		cout << "Idle" << endl;
		return;
	}

	if (rabbit.lastTimeSeen == frame) {
		float xrel = (x - (width / 2)) / (width / 2);
		//float ang = ((atan(xrel) * 180) / 3.1415) * factor + 90;
		float ang = (atan(xrel) * 180) / 3.1415;

		if (d > 2.0) {
			*throttle = 86;
			*steering = ang * 0.25 + 90;
		}
		else if (d > 1.0) {
			*throttle = 88;
			*steering = ang * 0.75 + 90;
		}
		else if (d > 0.5) {
			*throttle = 89;
			*steering = ang * 0.75 + 90;
		} else {
			*throttle = 91;
		}
		cout << "Update" << endl;
		cout << "a:" << ang << " d:" << d << endl;
		return;
	}

	if ((frame - rabbit.lastTimeSeen) < 3 && d > 1.5) {
		if (*steering < 90) {
			(*steering) += 1;
		}
		if (*steering > 90) {
			(*steering) -= 1;
		}
		*throttle = 88;
		cout << "Keep going" << endl;
		return;
	}

	//*steering = 90;
	*throttle = 91;
	cout << "Idle" << endl;
}
