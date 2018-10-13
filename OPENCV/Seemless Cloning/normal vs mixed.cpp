#include <opencv\cv.hpp>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

int main()
{
	// Read images : src image will be cloned into dst
	Mat src = imread("images/tes.jpg");
	Mat dst = imread("images/tes2.jpg");


	// Create an all white mask
	Mat src_mask = 255 * Mat::ones(src.rows, src.cols, src.depth());
	Mat src_mask2 = 255 * Mat::ones(src.rows, src.cols, src.depth());


	// The location of the center of the src in the dst
	Point center(dst.cols / 2, dst.rows / 2);

	// Seamlessly clone src into dst and put the results in output
	Mat normal_clone;
	Mat mixed_clone;

	seamlessClone(src, dst, src_mask, center, normal_clone,NORMAL_CLONE );
	seamlessClone(src, dst, src_mask2, center, mixed_clone, MIXED_CLONE);

	// Show result
	imshow("mask", src_mask);
	imshow("normal_clone", normal_clone);
	imshow("mixed_clone", mixed_clone);

	imwrite("images/normal_clone.jpg", normal_clone);
	imwrite("images/mixed_clone.jpg", mixed_clone);

	cvWaitKey(); //it waits for the given number of miliseconds for a key press
	return 0;
}