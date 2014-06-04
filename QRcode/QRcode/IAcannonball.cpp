#include "IAcannonball.h"

void buildDoors(struct door** d, int* IDs, int size) {
	struct door* prev = NULL;
	struct door* current = (struct door*)malloc(sizeof(struct door));
	*d = current;
	for (int i = 0; i < size;) {
		current->left = IDs[i++];
		current->right = IDs[i++];
		current->next = (struct door*)malloc(sizeof(struct door));
		prev = current;
		current = current->next;
	}
	free(current);
	prev->next = *d;
}

IAcannonball::IAcannonball():
frame(0),
idle(0)
{
	elements.clear();
	int  doors[] = {
		23731, 30339,
		56039, 27288,
		44387, 21676,
		 2248, 18244,
		59588, 56536,
		42425, 23647,
		16473, 17886,
		11806, 42534,
		37211, 64661,
		22662, 38006
	};
	buildDoors(&currentDoor, doors, 20);
}


IAcannonball::~IAcannonball()
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

void IAcannonball::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {

	//Update our map
	frame++;
	for (std::vector<aruco::Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		elements[it->id].marker = *it;
		elements[it->id].lastTimeSeen = frame;
	}

	if (idle > 100 && !(*TheMarkers).empty()) {
		while (currentDoor->left != (*TheMarkers)[0].id && currentDoor->right != (*TheMarkers)[0].id) {
			currentDoor = currentDoor->next;
		}
		idle = 0;
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

	if (left.lastTimeSeen == 0 && right.lastTimeSeen == 0) {
		idle++;
		*steering = 90;
		*throttle = 91;
		std::cout << " Idle" << endl;
		return;
	}

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

	//*steering = 90;
	*throttle = 91;
	idle++;
	std::cout << " Idle" << endl;
}