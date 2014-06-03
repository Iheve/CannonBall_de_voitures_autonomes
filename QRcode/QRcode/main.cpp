#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <zbar.h>
#include <iostream>
#include <map>
#include <Windows.h>
#include "SerialClass.h"
#include "aruco.h"
#include "highlyreliablemarkers.h"
#include "cvdrawingutils.h"
#include "mqtt_sender.h"
using namespace std;
using namespace zbar;
using namespace cv; 
using namespace aruco;

/**
 * Prompt the user to select the webcam number
 * Most of the time -1, 0, 1 or 2
 * switch eventually to HD
 */
void selectCam(CvCapture** capture, bool hd) {
	int cam = 0;
	while (!(*capture = cvCaptureFromCAM(cam))) {
		cout << "Capture failure\n";
		cout << "Enter camera number : ";
		cin >> cam;
	}

	if (hd) {
		cout << "switching to 1920x1080\n";
		cvSetCaptureProperty(*capture, CV_CAP_PROP_FRAME_WIDTH, 1920);
		cvSetCaptureProperty(*capture, CV_CAP_PROP_FRAME_HEIGHT, 1080);
		cout << "running !\n";
	}
}

struct element {
	vector<Point> corners;
	Point pos;
	Size2f size;
};

/**
 * Compute image symbols
 */
void compute(Image *image, map<string, struct element> *elements, Mat *img) {
	for (Image::SymbolIterator symbol = (*image).symbol_begin();
		symbol != (*image).symbol_end();
		++symbol) {
		vector<Point> vp;
		/*
		cout << "decoded " << symbol->get_type_name()
		<< " symbol \"" << symbol->get_data() << '"' << " ";
		*/

		int n = symbol->get_location_size();
		int pos_x = 0;
		int pos_y = 0;
		for (int i = 0; i < n; i++){
			vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			pos_x += symbol->get_location_x(i);
			pos_y += symbol->get_location_y(i);
		}
		pos_x /= n;
		pos_y /= n;


		RotatedRect r = minAreaRect(vp);
		Point2f pts[4];
		r.points(pts);

		for (int i = 0; i < 4; i++){
			line(*img, vp[i], vp[(i + 1) % 4], Scalar(255, 0, 0), 3);
		}
		//cout << "pos=" << pos_x << "x" << pos_y << " size=" << r.size.width << "x" << r.size.height << endl;



		(*elements)[symbol->get_data()].corners = vp;
		(*elements)[symbol->get_data()].pos = Point(pos_x, pos_y);
		(*elements)[symbol->get_data()].size = r.size;
	}
}

/**
 * update steering
 * use factor to adjust steering amplitude
 */
void getSteering(vector<Marker>* TheMarkers, int* steering, int* throttle, int width, float factor) {
	float x = 0;

	*throttle = 91;
	for (std::vector<Marker>::iterator it = (*TheMarkers).begin(); it != (*TheMarkers).end(); it++) {
		if (it->id == 18244) {
			x = it->getCenter().x;
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
			//cout << "d:" << d<<endl;
			//cout << "x:" << it->Rvec.ptr<float>(0)[0] << "y:" << it->Rvec.ptr<float>(0)[1] << "z:" << it->Rvec.ptr<float>(0)[2] << endl;
			//cv::waitKey(10000);
			//*steering = ((it->Rvec.ptr<float>(0)[2]) * 180 / 3.1415 + 90) * factor;
			break;
		}
	}
	/*
	//float x = (*elements)["Thibaut"].pos.x;
	if (x != 0) {
		float xrel = (x - (width / 2)) / (width / 2);
		float ang = ((atan(xrel) * 180) / 3.1415 + 90) * factor;
		*steering = ang;
	}
	*/

}

void initArduino(Serial** arduin) {
	*arduin = new Serial("\\\\.\\COM5"); // adjust as needed

	if ((*arduin)->IsConnected())
		cout << "Connection to the arduino OK" << endl;
}

class mqtt_sender *sender;
int rc;
bool connected = true;

void publish_to_mqtt(char *topic, char *message) {
	if (connected) {
		rc = sender->loop();
		if (rc){
			sender->reconnect();
		}
		sender->send_message(topic, message);
	}
}

/**
 * Send steering and throttle angles to the arduino
 */
void sendCommand(Serial** arduin, int steering, int throttle) {
	if ((*arduin)->IsConnected()) {
		char buff;
		// Send steering
		buff = (char)(steering);
		if (!(*arduin)->WriteData(&buff, 1)) {
			cout << "Steering write fail !" << endl;
		}
		// Send throttle
		buff = (char)(throttle);
		if (!(*arduin)->WriteData(&buff, 1)) {
			cout << "Throttle write fail !" << endl;
		}
		cout << "Command sent" << endl;
		publish_to_mqtt(TOPIC_STEER, (char*) std::to_string(steering).c_str());
		publish_to_mqtt(TOPIC_THROT, (char*) std::to_string(throttle).c_str());
		cout << "Metric send to server" << endl;
	}
}

int main(void) {
	mosqpp::lib_init();

	if (!(sender = new mqtt_sender("sender", "localhost", 1883))) {
		cout << "WARNING: unable to connect to MQTT, logging disabled." << endl;
		connected = false;
	}
	else {
		cout << "Connected to MQTT." << endl;
	}

	publish_to_mqtt("presence", "Hello mqtt");

	int steering = 90;
	int throttle = 91;
	
	Serial* arduin;
	initArduino(&arduin);

	String TheDict = "dict.yml";
	String TheCamParam = "camparam.yml";
	float TheMarkerSize = 0.042;

	VideoCapture TheVideoCapturer;
	vector<Marker> TheMarkers;
	Mat TheInputImage, TheInputImageCopy;
	CameraParameters TheCameraParameters;
	MarkerDetector MDetector;
	
	TheVideoCapturer.open(0);
	
	if (!TheVideoCapturer.isOpened()) {
		cerr << "Could not open video" << endl;
		return -1;

	}

	Dictionary D;
	if (D.fromFile(TheDict) == false) {
		cerr << "Could not open dictionary" << endl;
		return -1;
	};
	if (D.size() == 0) {
		cerr << "Invalid dictionary" << endl;
		return -1;
	};

	HighlyReliableMarkers::loadDictionary(D);


	//read first image to get the dimensions
	TheVideoCapturer >> TheInputImage;

	//read camera parameters if passed
	TheCameraParameters.readFromXMLFile(TheCamParam);
	TheCameraParameters.resize(TheInputImage.size());

	//Configure other parameters
	//if (ThePyrDownLevel > 0)
	//	MDetector.pyrDown(ThePyrDownLevel);


	MDetector.setMakerDetectorFunction(aruco::HighlyReliableMarkers::detect);
	MDetector.setThresholdParams(21, 7);
	MDetector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
	MDetector.setWarpSize((D[0].n() + 2) * 8);
	MDetector.setMinMaxSize(0.005, 0.5);


	cv::namedWindow("in", 1);

	int index = 0;
	do {
		TheVideoCapturer.retrieve(TheInputImage);
		//Detection of markers in the image passed
		MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);

		//print marker info and draw the markers in image
		TheInputImage.copyTo(TheInputImageCopy);
		for (unsigned int i = 0; i < TheMarkers.size(); i++) {
			//cout << TheMarkers[i] << endl;
			TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);
		}
		getSteering(&TheMarkers, &steering, &throttle, TheInputImage.size().width, 0.75);
		//cout << steering << endl;
		//sendCommand(&arduin, steering, throttle);
		sendCommand(&arduin, steering, throttle);

		//show input with augmented information and  the thresholded image
		cv::imshow("in", TheInputImageCopy);
		cv::waitKey(20);
	} while (TheVideoCapturer.grab());

}