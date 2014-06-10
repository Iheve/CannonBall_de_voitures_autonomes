package fr.ensimag.cannonball;

import org.opencv.core.Mat;
import org.opencv.core.Size;

public class CameraParameters {
	
	//#include "exports.h"
	//#include <opencv2/core/core.hpp>

	// 3x3 matrix (fx 0 cx, 0 fy cy, 0 0 1)
	Mat  CameraMatrix;
	//4x1 matrix (k1,k2,p1,p2)
	Mat  Distorsion;
	//size of the image
	Size CamSize;

}
