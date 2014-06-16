#include "AIRabbit.h"
#include <iostream>
#include "Windows.h"

using namespace aruco;

AIRabbit::AIRabbit(int argc, char *argv[]) :
frame(0)
{
	elements.clear();
	if (argc < 1) {
		std::cerr << "AI  Rabbit usage : <target>" << std::endl;
		std::cerr << "<target> : id of the target marker" << std::endl;
		Sleep(10000);
		exit(-1);
	}
	target = atoi(argv[0]);
	std::cout << "Following " << target << endl;
}


AIRabbit::~AIRabbit()
{
}

void AIRabbit::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {

	//Update our map
	frame++;
	for (std::vector<Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		elements[it->id].marker = *it;
		elements[it->id].lastTimeSeen = frame;
	}

	struct element rabbit = elements[target];
	float x = rabbit.marker.getCenter().x;
	float d = rabbit.marker.Tvec.ptr<float>(0)[2];

    //No marker ever seen
	if (rabbit.lastTimeSeen == 0) {
		*steering = 90;
		*throttle = 91;
		cout << "Idle" << endl;
		return;
	}

    //Best case : the marker is in view
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

    //Marker seen less than 3 frames ago
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
