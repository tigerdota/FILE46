#include <opencv\cv.hpp>
#include <opencv\highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

	// Read image 
	Mat src = imread("threshold.png", IMREAD_GRAYSCALE);
	Mat dst1, dst2, dst3, dst4, dst5, dst6, dst7;
	imshow("source", src);

	// Basic threhold example 
	threshold(src, dst1, 0, 255, THRESH_BINARY);
	imshow("basic_thresh_binary", dst1);

	// Thresholding with maxval set to 128
	threshold(src, dst2, 0, 128, THRESH_BINARY);
	imshow("thresh_maxval", dst2);

	// Thresholding with threshold value set 127 
	threshold(src, dst3, 127, 255, THRESH_BINARY);
	imshow("threshold_minval127", dst3);

	// Thresholding using THRESH_BINARY_INV 
	threshold(src, dst4, 127, 255, THRESH_BINARY_INV);
	imshow("thresh_bininvert", dst4);

	// Thresholding using THRESH_TRUNC 
	threshold(src, dst5, 127, 255, THRESH_TRUNC);
	imshow("thresh_trunc", dst5);

	// Thresholding using THRESH_TOZERO 
	threshold(src, dst6, 127, 255, THRESH_TOZERO);
	imshow("thresh_tozero", dst6);

	// Thresholding using THRESH_TOZERO_INV 
	threshold(src, dst7, 127, 255, THRESH_TOZERO_INV);
	imshow("thresh_tozero_inv", dst7);

	waitKey(0);
}