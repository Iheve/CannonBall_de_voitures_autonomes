package fr.ensimag.cannonball;

import java.util.ArrayList;

import org.opencv.core.Mat;
import org.opencv.core.Point;

public class Marker extends ArrayList<Point>{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	//#include "exports.h"
	//#include "cameraparameters.h"

	/**\brief This class represents a marker. It is a vector of the fours corners ot the marker
	 *
	 */

	//class  ARUCO_EXPORTS Marker: public std::vector<cv::Point2f> //POINT2F ?!

	
    //id of  the marker
    int id;
    //size of the markers sides in meters
    float ssize;
    //matrices of rotation and translation respect to the camera
    Mat Rvec,Tvec;

}
