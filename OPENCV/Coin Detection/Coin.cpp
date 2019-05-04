#include <opencv\cv.hpp>
#include <opencv\highgui.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

void find_moments(Mat result_src);
void find_blob(Mat thr, Mat img, Mat &result_src);

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	Mat thr,dst,gry;

	if (!cap.isOpened())  // check if we succeeded
		return -1;


	for (;;)
	{
		Mat frame;
		Mat result_blob;
		cap >> frame; // get a new frame from camera
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

		Mat img(frame.rows, frame.cols, CV_8UC1, Scalar(255)); // make a white image
		cvtColor(frame, gry, cv::COLOR_RGB2GRAY);
		blur(gry,gry,Size(3,3));
		threshold(gry, thr, 127, 200, THRESH_BINARY_INV);

		find_blob(thr , img, result_blob);
		find_moments(result_blob);

		imshow("thresh_bininvert", thr);
		imshow("Camera", frame);
		if (waitKey(30) >= 0) break;
	}

	return 0;
}


void find_blob(Mat thr, Mat img, Mat &im_with_keypoints)
{
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 150;
	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 200;
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.6;
	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;
	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.3;
	// Storage for blobs
	vector<KeyPoint> keypoints;
	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	detector->detect(thr, keypoints);

	drawKeypoints(img, keypoints, im_with_keypoints, Scalar(0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//erode(im_with_keypoints, im_with_keypoints, Mat(), Point(-1, -1), 1, 1, 1);
	//dilate(im_with_keypoints, im_with_keypoints, Mat(), Point(-1, -1), 1, 1, 1);
	
	// Show blobs
	imshow("filterblob", im_with_keypoints);
}

void find_moments(Mat thr)
{
	Mat canny_output;
	Mat thr_scd,thr_gry;
	div_t m_total;
	vector<vector<Point>> contour;
	vector<Vec4i> hierarchy;

	//detect using canny filter
	cvtColor(thr, thr, CV_BGR2GRAY);
	blur(thr, thr, Size(3, 3));
	Canny(thr, canny_output, 1, 2, 3);
	imshow("canny", canny_output);
	
	//find contour
	findContours(canny_output, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE,Point(0, 0));

	//Get the moment
	vector<Moments> mu(contour.size());
	for (int i = 0; i < contour.size(); i++)
	{
		mu[i] = moments(contour[i], false);
	}

	//  Get the centroid of figures.
	vector<Point2f> mc(contour.size());
	for (int i = 0; i < contour.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//Drawing contour
	Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
	
	//Calculate total coin
	m_total = div(contour.size(), 4);
	//if (m_total.rem <= 4) cout << "total coin = " << m_total.quot << endl;
	cout << "total coin = " << m_total.quot << endl;

	for (int i = 0; i< contour.size(); i++)
	{
		Scalar color = Scalar(167, 151, 0);
		drawContours(drawing, contour, i, color, 1, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 2, color, -1, 7, 0);
	}
	
	imshow("results", drawing);
	
}


