#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <zbar.h>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace zbar;
using namespace cv;

int main(void){
	
	cvNamedWindow("result", 1);

	CvCapture* capture = 0;
	while (!(capture = cvCaptureFromCAM(0))) {
		printf("Capture failure\n");
	}
	
	cout << "running !\n";
	for (;;) {
		IplImage* iplImg = cvQueryFrame(capture);

		//cout << "new frame !\n";

		cvShowImage("result", iplImg);
		waitKey(20);

		ImageScanner scanner;
		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
		Mat img(iplImg);

		// Grayscale Convertion
		Mat grayscale;
		cvtColor(img, grayscale, CV_BGRA2GRAY);
		
		//cvShowImage("result", new IplImage(grayscale));
		//waitKey(20);
		
		int width = grayscale.cols;
		int height = grayscale.rows;
		uchar *raw = (uchar *)grayscale.data;
		// wrap image data
		Image image(width, height, "Y800", raw, width * height);
		// scan the image for barcodes
		int n = scanner.scan(image);
		if (n != 0)
			cout << n << endl;
		// extract results
		for (Image::SymbolIterator symbol = image.symbol_begin();
			symbol != image.symbol_end();
			++symbol) {
			vector<Point> vp;
			// do something useful with results
			cout << "decoded " << symbol->get_type_name()
				<< " symbol \"" << symbol->get_data() << '"' << " " << endl;
			/*
			int n = symbol->get_location_size();
			for (int i = 0; i < n; i++){
				vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			}
			RotatedRect r = minAreaRect(vp);
			Point2f pts[4];
			r.points(pts);
			for (int i = 0; i < 4; i++){
				line(grayscale, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);
			}
			cout << "Angle: " << r.angle << endl;
			*/
		}
		
		// clean up
		image.set_data(NULL, 0);
		
	}
}