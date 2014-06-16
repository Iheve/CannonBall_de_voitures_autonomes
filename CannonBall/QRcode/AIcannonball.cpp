#include "AICannonball.h"
#include <iostream>
#include <fstream>
#include <queue>
#include "Windows.h"

void buildDoors(struct door** d, std::queue<std::pair<int, int>> doors) {
	struct door* prev = NULL;
	struct door* current = (struct door*)malloc(sizeof(struct door));
	*d = current;
	for (; !doors.empty() ;) {
		current->left = doors.front().first;
		current->right = doors.front().second;
		doors.pop();
		current->next = (struct door*)malloc(sizeof(struct door));
		prev = current;
		current = current->next;
	}
	free(current);
	prev->next = *d;
}

AICannonball::AICannonball(int argc, char *argv[]) :
frame(0),
idle(0)
{
	if (argc < 1) {
		std::cerr << "AI  Cannonball usage : doors_file" << std::endl;
		Sleep(10000);
		exit(-1);
	}
	elements.clear();
	std::queue<std::pair<int, int>> doors;
	std::ifstream ifs;
	ifs.open(argv[0]);
	int left, right;
	while (!ifs.eof()) {
		ifs >> left;
		ifs >> right;
		if (ifs.good()) {
			doors.push(std::make_pair(left, right));
		}
	}
	buildDoors(&currentDoor, doors);
}


AICannonball::~AICannonball()
{
	struct door* current;
	struct door* next;
	current = currentDoor;
	do {
		next = current->next;
		free(current);
		current = next;
	} while (current != currentDoor);

}

void AICannonball::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {

	//Update our map
	frame++;
	for (std::vector<aruco::Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		elements[it->id].marker = *it;
		if (elements[it->id].lastTimeSeen != 0 && elements[it->id].lastTimeSeen == (frame - 1)){
			elements[it->id].valid = true;
		}
		else {
			elements[it->id].valid = false;
		}
		elements[it->id].lastTimeSeen = frame;
	}

    //Find a new marker in view if idle for too long.
	if (idle > 10 && !(*TheMarkers).empty()) {
		int id = -1;
		for (std::vector<aruco::Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
			if (!elements[it->id].valid) {
				continue;
			}
			if (id == -1) {
				id = it->id;
			}
			else if (elements[id].marker.Tvec.ptr<float>(0)[2] > elements[it->id].marker.Tvec.ptr<float>(0)[2]) {
				id = it->id;
			}
		}
		if (id != -1) {
			while (currentDoor->left != id && currentDoor->right != id) {
				currentDoor = currentDoor->next;
			}
			idle = 0;
		}
	}

	struct element left = elements[currentDoor->left];
	struct element right = elements[currentDoor->right];
	struct element nextLeft = elements[currentDoor->next->left];
	struct element nextRight = elements[currentDoor->next->right];
	
	float xLeft = left.marker.getCenter().x;
	float xRight = right.marker.getCenter().x;
	float dLeft = left.marker.Tvec.ptr<float>(0)[2];
	float dRight = right.marker.Tvec.ptr<float>(0)[2];
	float xNextLeft = nextLeft.marker.getCenter().x;
	float xNextRight = nextRight.marker.getCenter().x;
	float dNextLeft = nextLeft.marker.Tvec.ptr<float>(0)[2];
	float dNextRight = nextRight.marker.Tvec.ptr<float>(0)[2];

    //Go to the next door
	if (((frame - left.lastTimeSeen) > 2 || (frame - right.lastTimeSeen) > 2)
		&& ((frame - nextLeft.lastTimeSeen) == 0 || (frame - nextRight.lastTimeSeen) == 0)) {

		currentDoor = currentDoor->next;

		left = elements[currentDoor->left];
		right = elements[currentDoor->right];
		nextLeft = elements[currentDoor->next->left];
		nextRight = elements[currentDoor->next->right];

		xLeft = left.marker.getCenter().x;
		xRight = right.marker.getCenter().x;
		dLeft = left.marker.Tvec.ptr<float>(0)[2];
		dRight = right.marker.Tvec.ptr<float>(0)[2];
		xNextLeft = nextLeft.marker.getCenter().x;
		xNextRight = nextRight.marker.getCenter().x;
		dNextLeft = nextLeft.marker.Tvec.ptr<float>(0)[2];
		dNextRight = nextRight.marker.Tvec.ptr<float>(0)[2];
	}

	std::cout << currentDoor->left << "/" << currentDoor->right;

    //No marker ever seen
	if (left.lastTimeSeen == 0 && right.lastTimeSeen == 0) {
		idle++;
		*steering = 90;
		*throttle = 91;
		std::cout << " Idle" << endl;
		return;
	}

    //We can see both current markers
	if ((frame - left.lastTimeSeen) == 0 && (frame - right.lastTimeSeen) == 0) {
		idle = 0;
		float xrel = ((xLeft + xRight) / 2 - (width / 2)) / (width / 2);
		float ang = (atan(xrel) * 180) / 3.1415;

		std::cout << " seen";
		if (dLeft > 2.0 && dRight > 2.0) {
			*throttle = 86;
			*steering = ang * 0.25 + 90;
			 std::cout << " far away" << std::endl;
		}
		else if (dLeft > 1.0 && dRight > 1.0) {
			*throttle = 88;
			*steering = ang * 0.75 + 90;
			std::cout << " medium" << std::endl;
		}
		else if (dLeft > 0.5 && dRight > 0.5) {
			*throttle = 89;
			*steering = ang * 0.75 + 90;
			std::cout << " close" << std::endl;
		}
		else {
			*throttle = 91;
			std::cout << " very close" << std::endl;
		}
		return;
	}

    //See only left marker
	if ((frame - left.lastTimeSeen) == 0){
		idle = 0;
		float xrel = (xLeft - (width / 2)) / (width / 2);
		float ang = (atan(xrel) * 180) / 3.1415;

		*throttle = 88;
		std::cout << " left seen";
		if (dLeft > 2.0) {
			*steering = ang * 0.25 + 90 + 5;
			std::cout << " far away" << std::endl;
		}
		else if (dLeft > 1.0) {
			*steering = ang * 0.75 + 90 + 15;
			std::cout << " medium" << std::endl;
		}
		else if (dLeft > 0.5) {
			*throttle = 89;
			*steering = ang * 0.75 + 90 + 30;
			std::cout << " close" << std::endl;
		}
		else {
			*throttle = 91;
			std::cout << " very close" << std::endl;
		}
		return;
	}

    //See only right marker
	if ((frame - right.lastTimeSeen) == 0){
		idle = 0;
		float xrel = (xRight - (width / 2)) / (width / 2);
		float ang = (atan(xrel) * 180) / 3.1415;

		std::cout << " right seen";
		*throttle = 88;
		if (dRight > 2.0) {
			*steering = ang * 0.25 + 90 - 5;
			std::cout << " far away" << std::endl;
		}
		else if (dRight > 1.0) {
			*steering = ang * 0.75 + 90 - 15;
			std::cout << " medium" << std::endl;
		}
		else if (dRight > 0.5) {
			*throttle = 89;
			*steering = ang * 0.75 + 90 - 30;
			std::cout << " close" << std::endl;
		}
		else {
			*throttle = 91;
			std::cout << " very close" << std::endl;
		}
		return;
	}

	*throttle = 91;
	idle++;
	std::cout << " Idle" << endl;
}
