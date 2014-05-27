#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <zbar.h>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace zbar;
using namespace cv;

int main(void){
	
	CvCapture* capture = 0;
	int cam;
	cout << "Enter camera number : ";
	cin >> cam;
	while (!(capture = cvCaptureFromCAM(cam))) {
		cout << "Capture failure\n";
		cout << "Enter camera number : ";
		cin >> cam;
	}
	
	cout << "switching to 1920x1080\n";
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1920);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 1080);
	cout << "running !\n";

	cvNamedWindow("result", 1);

	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);


	for (;;) {
		IplImage* iplImg = cvQueryFrame(capture);

		Mat img(iplImg);

		// Grayscale Convertion
		Mat grayscale;
		cvtColor(img, grayscale, CV_BGRA2GRAY);
		
		int width = grayscale.cols;
		int height = grayscale.rows;
		uchar *raw = (uchar *)grayscale.data;
		// wrap image data
		Image image(width, height, "Y800", raw, width * height);
		// scan the image for barcodes
		int n = scanner.scan(image);
		if (n != 0)
			cout << n << " elements detected" << endl;
		// extract results
		for (Image::SymbolIterator symbol = image.symbol_begin();
			symbol != image.symbol_end();
			++symbol) {
			vector<Point> vp;
			// do something useful with results
			cout << "decoded " << symbol->get_type_name()
				<< " symbol \"" << symbol->get_data() << '"' << " ";
		
			int n = symbol->get_location_size();
			int pos_x = 0;
			int pos_y = 0;
			for (int i = 0; i < n; i++){
				//cout << "x=" << symbol->get_location_x(i) << " y=" << symbol->get_location_y(i) << " ";
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
				line(img, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);
			}
			//cout << "Angle: " << r.angle << endl;
			cout << "pos=" << pos_x << "x" << pos_y << " size=" << r.size.width << "x" << r.size.height << endl;
		}
		cvShowImage("result", new IplImage(img));
		waitKey(20);
		
		
		// clean up
		image.set_data(NULL, 0);
		
	}
}