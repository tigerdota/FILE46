#include <opencv\cv.hpp>
#include <opencv\highgui.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string>
//#include <atlstr.h> //serial communication

#include "SerialPort.h"



using namespace std;
using namespace cv;

void find_moments(Mat result_src);
void find_blob(Mat thr, Mat img, Mat &result_src);

VideoCapture cap(0); // open the default camera

char output[MAX_DATA_LENGTH]; //Serial Port
char incomingData[MAX_DATA_LENGTH];

//char* port = "\\\\.\\COM19";

char port[] = "\\\\.\\COM19";
//char *ptrToString = port;
char ON[] = "ON";
char OFF[] = "OFF";


int main(){
	Mat thr,dst,gry;
	char c;

	SerialPort arduino(port);
	if (arduino.isConnected()) {
		cout << "Connection Arduino Terhubung" << endl << endl;
	}
	else {
		cout << "Error in port name" << endl << endl;
	}


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
		//imshow("Camera", frame);
		char key = (char)cv::waitKey(30);
		if (key == 27) break;
		if (key == 97) {
			cout << "ON" << endl;
			arduino.writeSerialPort(ON, MAX_DATA_LENGTH);

		}
		if (key == 100) {
			cout << "OFF" << endl;
			arduino.writeSerialPort(OFF, MAX_DATA_LENGTH);
	
		}
		//if (waitKey(30) == 27) break;
	}

	return 0;
}


void find_blob(Mat thr, Mat img, Mat &im_with_keypoints)
{
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;
	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 900;
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.85;
	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.1;
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
	Mat frame2;
	cap >> frame2;
	Mat canny_output;
	Mat thr_scd,thr_gry;
	div_t m_total;
	vector<vector<Point>> contour;
	vector<Vec4i> hierarchy;
	
	//Init font
	String text  = "500";
	String text2 = "200";
	String text3 = "100";
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 1.8;


	//detect using canny filter
	cvtColor(thr, thr, CV_BGR2GRAY);
	blur(thr, thr, Size(3, 3));
	Canny(thr, canny_output, 1, 2, 3);
	imshow("canny", canny_output);               //show canny image
	
	//find contour
	findContours(canny_output, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE,Point(0, 0));

	//Get the moment
	vector<Moments> mu(contour.size());
	for (int i = 0; i < contour.size(); i++)
	{
		mu[i] = moments(contour[i], false);
	}

	//Drawing contour
	Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
	Scalar color = Scalar(0, 0, 255);

	//  Get the centroid of figures.
	vector<Point2f> mc(contour.size());
	vector<Point2f> mk(contour.size());
	
	int iterasi = 0;
	int savpoint[100];
	int jaraktot;

	for (int i = 0; i < contour.size(); i++)
	{
		int k;
		int j = contour.size() % 4;
		//cout << j << endl;
		if (j == 0) {
			//cout << i << endl;
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			//cout << "data = " << mc[i].x << "," << mc[i].y << endl;

			k = i + 1;
			if (k % 4 == 1 || k % 4 == 0) {
				double jarak1 = sqrt(pow((contour[i][1].x - mc[i].x), 2) + pow((contour[i][1].y - mc[i].y), 2));
				double jarak2 = sqrt(pow((contour[i][2].x - mc[i].x), 2) + pow((contour[i][2].y - mc[i].y), 2));
				double jarak3 = sqrt(pow((contour[i][3].x - mc[i].x), 2) + pow((contour[i][3].y - mc[i].y), 2));
				double jarak4 = sqrt(pow((contour[i][4].x - mc[i].x), 2) + pow((contour[i][4].y - mc[i].y), 2));
				double jarak5 = sqrt(pow((contour[i][5].x - mc[i].x), 2) + pow((contour[i][5].y - mc[i].y), 2));
				double jarak6 = sqrt(pow((contour[i][6].x - mc[i].x), 2) + pow((contour[i][6].y - mc[i].y), 2));
				double jarak7  = sqrt(pow((contour[i][7].x - mc[i].x), 2) + pow((contour[i][7].y - mc[i].y), 2));
				double jarak8  = sqrt(pow((contour[i][8].x - mc[i].x), 2) + pow((contour[i][8].y - mc[i].y), 2));
				double jarak9  = sqrt(pow((contour[i][9].x - mc[i].x), 2) + pow((contour[i][9].y - mc[i].y), 2));
				double jarak10 = sqrt(pow((contour[i][10].x - mc[i].x), 2) + pow((contour[i][10].y - mc[i].y), 2));

				cout << "k = " << k << endl;
				jaraktot = (jarak1 + jarak2 + jarak3 + jarak4 + jarak5 + jarak6 + jarak7 + jarak8 + jarak9 + jarak10) / 10;
				cout << "jarak = " << jaraktot << endl;

				//cout << "i =" << i << endl;
				//cout << "iterasi =" << iterasi << endl;
				savpoint[iterasi] = jaraktot; 
				mk[iterasi] = mc[i];
				
				iterasi++;
				

			} 
			
		}
	}

	
	for (int i = 0; i < iterasi; i++) {
		cout << "j =" << savpoint[i] << endl;
		circle(frame2, mk[i], 3, color, -1, 7, 0);
		
		int cek = 0;
		if (i % 2 == 0) {
			//coin 200
			if (cek == 0 && savpoint[i] == 21 && savpoint[i + 1] == 18) {
				putText(frame2, text2, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);
				cek++;
			}
			if (cek == 0 && savpoint[i] == 21 && savpoint[i + 1] == 17) {
				putText(frame2, text2, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			}
			if (cek == 0 && savpoint[i] == 20 && savpoint[i + 1] == 18) {
				putText(frame2, text2, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			}
			if (cek == 0 && savpoint[i] == 22 && savpoint[i + 1] == 19) {
				putText(frame2, text2, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			}


			//coin 500
			if (cek == 0 && savpoint[i] == 23 && savpoint[i + 1] == 20) {
				putText(frame2, text, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			}
			if (cek == 0 && savpoint[i] == 23 && savpoint[i + 1] == 29) {
				putText(frame2, text, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			}
			//if (cek == 0 && savpoint[i] == 22 && savpoint[i + 1] == 19) {
				putText(frame2, text, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

			//}
			if (cek == 0 && savpoint[i] == 21 && savpoint[i + 1] == 18) {
				putText(frame2, text, mk[i], fontFace, fontScale, Scalar::all(0), thickness, 2);
			}

			//coin 100
			//if (jaraktot <= 20) putText(frame2, text3, mc[i], fontFace, fontScale, Scalar::all(0), thickness, 2);

		} 
	}

	
	//Calculate total coin
	//m_total = div(contour.size(), 4);
	//if (m_total.rem <= 4) cout << "total coin = " << m_total.quot << endl;
	//cout << "total coin = " << m_total.quot << endl;


	//Drawing on image
	//for (int i = 0; i< contour.size(); i++)
	//{
		//Scalar color = Scalar(0, 0, 255);
		//drawContours(drawing, contour, i, color, 1, 8, hierarchy, 0, Point());
		//circle(drawing, mc[i], 20, color, 2, 8 ,0);
		//circle(drawing, contour[i][0], 2, color, -1, 7, 0);
	//}
	
	imshow("results", frame2);
	
}
