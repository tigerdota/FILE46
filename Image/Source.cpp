#include <opencv\cv.hpp>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

int main()
{
	IplImage *img = cvLoadImage("Shrimp.jpg"); // to load image from your folder
	cvNamedWindow("Image:", 1); // give a name to your console windows
	cvShowImage("Image:", img); // show your image

	cvWaitKey(); //it waits for the given number of miliseconds for a key press
	cvDestroyWindow("Image:"); // close your console windows
	cvReleaseImage(&img); // releases the memory allocated to an image

	return 0;
}