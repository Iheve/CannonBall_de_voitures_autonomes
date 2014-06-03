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
#include "IA.h"
#include "IARabbit.h"
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
		rc = sender->loop_start();
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
		char buff [2];
		// Send steering
		buff[0] = (char)(steering);
		/*
		if (!(*arduin)->WriteData(&buff, 1)) {
			cout << "Steering write fail !" << endl;
		}*/
		// Send throttle
		buff[1] = (char)(throttle);
		if (!(*arduin)->WriteData(buff, 2)) {
			cout << "Throttle write fail !" << endl;
		}
		cout << "Command sent" << endl;
	}
}

void sendMetrics(int steering, int throttle) {
	publish_to_mqtt(TOPIC_STEER, (char*)std::to_string(steering).c_str());
	publish_to_mqtt(TOPIC_THROT, (char*)std::to_string(throttle).c_str());
	cout << "Metrics sent" << endl;
}

int main(int argc, char *argv[]) {
	mosqpp::lib_init();
	if (argc >= 2) {
		cout << "Connecting to " << argv[1] << endl;
		sender = new mqtt_sender("sender", argv[1], 1883);
	}
	else {
		cout << "Connecting to localhost" << endl;
		sender = new mqtt_sender("sender", "localhost", 1883);
	}

	if (!sender) {
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

	IARabbit ia;

	int index = 0;
	do {
		TheVideoCapturer.retrieve(TheInputImage);
		TheVideoCapturer.grab();
		//Detection of markers in the image passed
		MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);

		//print marker info and draw the markers in image
		TheInputImage.copyTo(TheInputImageCopy);
		for (unsigned int i = 0; i < TheMarkers.size(); i++) {
			//cout << TheMarkers[i] << endl;
			TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);
		}
		ia.getCommand(&TheMarkers, &steering, &throttle, TheInputImage.size().width);
		sendCommand(&arduin, steering, throttle);

		sendMetrics(steering, throttle);

		//show input with augmented information and  the thresholded image
		cv::imshow("in", TheInputImageCopy);
		cv::waitKey(10);
	} while (1);

}