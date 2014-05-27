#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <zbar.h>
#include <iostream>
using namespace std;
using namespace zbar;
using namespace cv;

int main(void){
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    // obtain image data
    char file[256];
    cin>>file;
    Mat img = imread(file,0);
    Mat imgout;
    cvtColor(img,imgout,CV_GRAY2RGB);
    int width = img.cols;
    int height = img.rows;
    uchar *raw = (uchar *)img.data;
    // wrap image data
    Image image(width, height, "Y800", raw, width * height);
    // scan the image for barcodes
    int n = scanner.scan(image);
    // extract results
    for(Image::SymbolIterator symbol = image.symbol_begin();
            symbol != image.symbol_end();
            ++symbol) {
        vector<Point> vp;
        // do something useful with results
        cout << "decoded " << symbol->get_type_name()
            << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++){
            vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }
        RotatedRect r = minAreaRect(vp);
        Point2f pts[4];
        r.points(pts);
        for(int i=0;i<4;i++){
            line(imgout,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
        }
        cout<<"Angle: "<<r.angle<<endl;
    }
    //imshow("imgout.jpg",imgout);
    // clean up
    image.set_data(NULL, 0);
    waitKey();
}


/*
int main(int argc, char** argv)
{ 
	IplImage* img = cvLoadImage("generate.png"); //change the name (image.jpg) according to your Image filename.
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
	return 0;
}
*/