#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <Windows.h>
#include "SerialClass.h"
#include "aruco.h"
#include "highlyreliablemarkers.h"
#include "cvdrawingutils.h"
#include "IARabbit.h"
#include "mqtt_sender.h"

using namespace cv;		
using namespace aruco;

//Arduino
Serial* arduin;
char* port = "\\\\.\\COM5";

//MQTT
mqtt_sender *sender;
char* mqtt_host = "localhost";

//Current state
int steering = 90;
int throttle = 91;

//aruco
VideoCapture TheVideoCapturer;
Mat TheInputImage, TheInputImageCopy;
vector<Marker> TheMarkers;
CameraParameters TheCameraParameters;
MarkerDetector MDetector;
Dictionary D;
String TheDict = "dict.yml";
String TheCamParam = "camparam.yml";
float TheMarkerSize = 0.089f;


/**
 * Initialization of the arduino
 */
void initArduino() {
	arduin = new Serial(port);

	if (arduin->IsConnected())
		std::cout << "Connection to the arduino OK" << std::endl;
}

void initMQTT() {
	std::cout << "Connecting to " << mqtt_host << std::endl;
	sender = new mqtt_sender("sender", mqtt_host, 1883);
	sender->publish_to_mqtt("presence", "Hello mqtt");
}

void initAruco() {
	//Open webcam
	TheVideoCapturer.open(0);
	TheVideoCapturer.set(CV_CAP_PROP_FRAME_WIDTH, 1920 / 3);
	TheVideoCapturer.set(CV_CAP_PROP_FRAME_HEIGHT, 1080 / 3);
	if (!TheVideoCapturer.isOpened()) {
		cerr << "Could not open video" << std::endl;
		exit(-1);
	}

	//Load dictionary
	if (D.fromFile(TheDict) == false) {
		cerr << "Could not open dictionary" << std::endl;
		exit(-1);
	}
	if (D.size() == 0) {
		cerr << "Invalid dictionary" << std::endl;
		exit(-1);
	}
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
	MDetector.setMinMaxSize(0.005f, 0.5f);

	cv::namedWindow("in", 1);
}

/**
 * Send steering and throttle angles to the arduino
 */
void sendCommand(Serial** arduin, int steering, int throttle) {
	if ((*arduin)->IsConnected()) {
		char buff [2];
		buff[0] = (char)(steering);
		buff[1] = (char)(throttle);
		if (!(*arduin)->WriteData(buff, 2)) {
			std::cout << "Serial write fail !" << std::endl;
		}
	}
}

/**
 * Send metrics to the MQTT broker
 */
void sendMetrics(int steering, int throttle, double laps, double avg) {
	sender->publish_to_mqtt(TOPIC_STEER, (char*)std::to_string(steering).c_str());
	sender->publish_to_mqtt(TOPIC_THROT, (char*)std::to_string(throttle).c_str());
	sender->publish_to_mqtt(TOPIC_LAPS, (char*)std::to_string(laps).c_str());
	sender->publish_to_mqtt(TOPIC_AVG, (char*)std::to_string(avg).c_str());
	//cout << "SENT!" << endl;
}



int main(int argc, char *argv[]) {
	
	//Arduino
	initArduino();

	//MQTT
	mosqpp::lib_init();
	initMQTT();

	//Aruco
	initAruco();

	IARabbit ia;
	int index = 0;
	double tick = (double)getTickCount();
	double laps = 0;
	float total = 0;
	do {
		//Calculate processing time
		index++;
		laps = (((double)getTickCount() - tick) / getTickFrequency() * 1000);
		total += (float) laps;
		std::cout << "time enlapsed : " << laps << " ms" << " (avg : " << total / index << ")" << std::endl;
		tick = (double)getTickCount();

		//Get a new frame
		TheVideoCapturer.retrieve(TheInputImage);
		TheVideoCapturer.grab();

		//Detection of markers in the image passed
		MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);

		//print marker info and draw the markers in image
		TheInputImage.copyTo(TheInputImageCopy);
		for (unsigned int i = 0; i < TheMarkers.size(); i++) {
			TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);
		}

		//Get steering and throttle from IA
		ia.getCommand(&TheMarkers, &steering, &throttle, TheInputImage.size().width);

		//Send command on the serial bus
		sendCommand(&arduin, steering, throttle);

		//Send metrics to the MQTT brocker
		sendMetrics(steering, throttle, laps, total/index);

		//show input with augmented information
		cv::imshow("in", TheInputImageCopy);
		cv::waitKey(10);
	} while (1);

}