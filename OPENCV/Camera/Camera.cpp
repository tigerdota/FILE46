#include <opencv\cv.hpp>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	namedWindow("Camera", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		imshow("Camera", frame);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}