#include "handDetection.h"

#include <iostream>
#define PI 3.14159265

using namespace std;
using namespace cv;

int hmin = 0,	smin = 0,	vmin = 120;
int hmax = 35,	smax = 150,	vmax = 250;


handDetection::handDetection()
{
	VideoCapture webcam;
	webcam.open(0);

	namedWindow("Trackbars", (614, 200));
	createTrackbar("Hue min", "Trackbars", &hmin, 179);
	createTrackbar("Hue max", "Trackbars", &hmax, 179);
	createTrackbar("Sat min", "Trackbars", &smin, 255);
	createTrackbar("Sat max", "Trackbars", &smax, 255);
	createTrackbar("Val min", "Trackbars", &vmin, 255);
	createTrackbar("Val max", "Trackbars", &vmax, 255);
	
	while (1) {
		getCommand(webcam);
		waitKey(1);
	}
}

handDetection::~handDetection()
{
}

string handDetection::getCommand(VideoCapture webcam) {
	Mat cameraFrame, blurFrame, closedFrame, hsvFrame, thresholdFrame;
	string command = "nothing-new";

	webcam.read(cameraFrame);
	//switch the RGB to HSV space
	cvtColor(cameraFrame, hsvFrame, COLOR_BGR2HSV);

	//Adjust colors to find hand
	inRange(hsvFrame, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdFrame);

	//Erodes and Dilates for noise reduction
	imshow("treshold", thresholdFrame);
	noiseReduction(thresholdFrame);
	imshow("threshold dilated", thresholdFrame);

	//track the hand, put the bounding box around the hand
	//calculate the center point of the hand
	command = trackHand(thresholdFrame, cameraFrame);

	imshow("Hand_Gesture_Detection", cameraFrame);

	//release the memory
	cameraFrame.release();

	return command;
}

//Reduces noice in image
void handDetection::noiseReduction(Mat& frame) {
	erode(frame, frame, Mat());
	erode(frame, frame, Mat());
				  
	dilate(frame, frame, Mat());
	dilate(frame, frame, Mat());
	dilate(frame, frame, Mat());
	dilate(frame, frame, Mat());

}

//the important function to track the hand, the algorithm is described in the report
string handDetection::trackHand(Mat src, Mat& dest) {

	Rect boundRect;
	int largestObj;
	vector<vector<Point> > contours; 
	vector<vector<Point> > contoursSet(contours.size());
	vector<Vec4i> hierarchy;
	vector<Point> convexHullPoint;
	vector<Point> fingerPoint;
	Point centerP;
	int numObjects = 0;
	double area = 0;
	double maxArea = 0;
	bool handFound = false;

	//initialize default command
	String resultMsg = "nothing-new";

	//find all the contours in the threshold Frame
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	numObjects = hierarchy.size();
	for (int i = 0; i < contours.size(); i++) {
		Mat tempContour = Mat(contours[i]);
		area = contourArea(tempContour);
		if (area > maxArea) {
			maxArea = area;
			largestObj = i;
		}
	}
	if (maxArea > 4000) {

		handFound = true;
		boundRect = boundingRect(contours[largestObj]);

		//draw the boundary of the object
		drawContours(dest, contours, largestObj, Scalar(0, 0, 255), 3, 8, hierarchy);
		//find the convex points for the largest object which is hand
		convexHull(contours[largestObj], convexHullPoint, true, true);

		//use moment method to find the center point
		Moments moment = moments(Mat(contours[largestObj]), true);
		int centerX = moment.m10 / moment.m00;
		int centerY = moment.m01 / moment.m00;
		Point centerPoint(centerX, centerY);
		centerP = centerPoint;
		Point printPoint(centerX, centerY + 15);
		Point printPoint1(boundRect.x, boundRect.y);
		circle(dest, centerPoint, 8, Scalar(255, 0, 0), FILLED);

		//put the BoundingBox in the contour region
		rectangle(dest, boundRect, Scalar(0, 0, 255), 2, 8, 0);

		if (handFound) {

			int countHullPoint = convexHullPoint.size();
			int maxdist = 0;
			int pos = 0;
			for (int i = 1; i < countHullPoint; i++) {

				pos = i;
				if (centerP.y >= convexHullPoint[i].y && centerP.y >= convexHullPoint[pos].y) {

					pos = i;
					int dist = (centerP.x - convexHullPoint[i].x) ^ 2 + (centerP.y - convexHullPoint[i].y) ^ 2;
					if (abs(convexHullPoint[i - 1].x - convexHullPoint[i].x) < 12) {

						if (dist > maxdist) {
							maxdist = dist;
							pos = i;
						}
					}
					else if (i == 0 || abs(convexHullPoint[i - 1].x - convexHullPoint[i].x) >= 12) {

						fingerPoint.push_back(convexHullPoint[pos]);
						line(dest, centerP, convexHullPoint[pos], Scalar(0, 255, 0), 3, 8, 0);
						circle(dest, convexHullPoint[pos], 8, Scalar(255, 0, 0), FILLED);
						pos = i;
					}
				}
			}

			//get the size the fingers
			int countFinger = fingerPoint.size();
			if (countFinger > 5) {
				countFinger = 5;
			}
			
			sendResult(resultMsg);
			putText(dest, to_string(countFinger), printPoint, 1, 5, Scalar(0, 255, 0), 1, 5, false);
		}
	}
	return resultMsg;
}

////send out the result signal
void handDetection::sendResult(String msg) {
	cout << "Command: " << msg << endl;
}