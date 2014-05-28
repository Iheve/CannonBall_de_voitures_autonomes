#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <zbar.h>
#include <iostream>
#include <map>
#include <Windows.h>
using namespace std;
using namespace zbar;
using namespace cv;

/**
 * Prompt the user to select the webcam number
 * Most of the time -1, 0, 1 or 2
 * switch eventually to HD
 */
void selectCam(CvCapture** capture, bool hd) {
	int cam;
	cout << "Enter camera number : ";
	cin >> cam;
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


void getSteering(map<string, struct element> *elements, int* steering, Mat *img, float width, float height, float factor) {
	float x = (*elements)["Thibaut"].pos.x;
	if (x != 0) {
		line(*img, Point(x, 0), Point(x, height), Scalar(255, 0, 0), 3);
		float xrel = (x - (width / 2)) / (width / 2);
		float ang = ((atan(xrel) * 180) / 3.1415 + 90) * factor;
		*steering = ang;
	}

}

int main(void){

	CvCapture* capture = 0;

	selectCam(&capture, false);

	cvNamedWindow("result", 1);

	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	int steering = 90;

	map<string, struct element> elements;

	for (;;) {
		// Get a new frame
		IplImage* iplImg = cvQueryFrame(capture);
		Mat img(iplImg);

		// Grayscale Convertion
		Mat grayscale;
		cvtColor(img, grayscale, CV_BGRA2GRAY);

		// Raw data image
		int width = grayscale.cols;
		int height = grayscale.rows;
		uchar *raw = (uchar *)grayscale.data;

		// wrap image data
		Image image(width, height, "Y800", raw, width * height);

		// scan the image for barcodes
		int n = scanner.scan(image);
				
		compute(&image, &elements, &img);
		
		getSteering(&elements, &steering, &img, width, height, 1);

		// Show markers on image
		cvShowImage("result", new IplImage(img));
		waitKey(20);

		//cout << "Elements in map : " << elements.size() << endl;
		cout << "Steering : " << steering << endl;

		// clean up
		image.set_data(NULL, 0);
	}
}