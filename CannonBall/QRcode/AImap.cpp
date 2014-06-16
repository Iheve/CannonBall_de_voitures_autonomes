#include "AImap.h"
#include <iostream>

using namespace aruco;

AImap::AImap()
{
	cout << "AImap : press enter to start" << endl;
	std::cin.get();
	processing = false;
}


AImap::~AImap()
{
}

int  marquers[] = {
	23731,
	56039,
	44387,
	59588,
	42425,
	16473,
	11806,
	37211,
};

void AImap::getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width) {
	frame++;

	*throttle = 88; // default

	if (processing) {
		//TODO export as function to win a frame
		cout << "Processing" << endl;
		//Keep the process
		*steering = 45;
		processing_frame_remaining--;
		if (processing_frame_remaining <= 0) { processing = false; }
	}
	else {
		
		*steering = 90; // default

		int current_marker = -1;
		//TODO pick the closest marker
		for (std::vector<Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
			//Assuming there's only one
			elements[it->id].marker = *it;
			elements[it->id].lastTimeSeen = frame;
			bool b = false;
			for (int i = 0; i < 10; i++) {
				if (marquers[i] == it->id) {
					current_marker = it->id;
					cout << "I know " << current_marker << endl;
					break;
				}
			}
			
		}
		
		if (following) {
			if (elements[last_marker].lastTimeSeen != frame) {
				cout << "Lost the followed marker " << last_marker << endl;
				//We lost the marker followed
				processing = true;
				following = false;
				processing_frame_remaining = 10;
				return;
			}
		}

		if (current_marker == -1) {
			//Keep going
			cout << "No stable marker" << endl;
			return;
		}

		// This marker is stable 
		cout << "Stable marker : ";
		last_marker = current_marker;
		following = true;
		// compute distance
		float d = elements[current_marker].marker.Tvec.ptr<float>(0)[2];
		// Not too close ? Center the car
		cout << "Distance " << d << endl;
		if (d > 0.5f) {
			cout << "Centering.." << endl;
			float x = elements[current_marker].marker.getCenter().x;
			float xrel = (x - (width / 2)) / (width / 2);
			float ang = (atan(xrel) * 180) / 3.1415;

			if (d > 2.0) {
				*steering = ang * 0.25 + 90;
			}
			else if (d > 1.0) {
				*steering = ang * 0.75 + 90;
			}
			else if (d > 0.75) {
				*steering = ang * 0.75 + 90;
			}
		}
		// Too close, change input
		//TODO LOST
		else {
			cout << "TOO CLOSE, start processing.." << endl;
			processing = true;
			following = false;
			processing_frame_remaining = 10;
		}
	}
}
