#include "handDetection.h"

#include <iostream>

using namespace std;
using namespace cv;

int hmin = 0,	smin = 0,	vmin = 120;
int hmax = 35,	smax = 150,	vmax = 250;

int fingerCount;
int webcamNr;
VideoCapture webcam;

HandDetection::HandDetection(int givenWebcamNr)
{
	webcamNr = givenWebcamNr;

	namedWindow("Trackbars", (614, 200));
	createTrackbar("Hue min", "Trackbars", &hmin, 179);
	createTrackbar("Hue max", "Trackbars", &hmax, 179);
	createTrackbar("Sat min", "Trackbars", &smin, 255);
	createTrackbar("Sat max", "Trackbars", &smax, 255);
	createTrackbar("Val min", "Trackbars", &vmin, 255);
	createTrackbar("Val max", "Trackbars", &vmax, 255);
	
}

int HandDetection::findFingers() {

	webcam.open(webcamNr);

	int fingerCountReturn = 0;
	int previousFingerCount = 0;
	int fingerLoopsThreshold = 20;
	int currentFingerLoops = 0;

	while (1) {
		Mat cameraFrame, hsvFrame, thresholdFrame;

		webcam.read(cameraFrame);
		//switch the RGB to HSV space
		cvtColor(cameraFrame, hsvFrame, COLOR_BGR2HSV);

		//Adjust colors to find hand
		inRange(hsvFrame, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdFrame);

		//Erodes and Dilates for noise reduction
		imshow("treshold", thresholdFrame);
		noiseReduction(thresholdFrame);
		imshow("threshold noise reduced", thresholdFrame);

		//track the hand, put the bounding box around the hand
		//calculate the center point of the hand
		fingerCountReturn = trackHand(thresholdFrame, cameraFrame);

		if (fingerCountReturn == previousFingerCount) {
			currentFingerLoops++;
		}
		else currentFingerLoops = 0;

		previousFingerCount = fingerCountReturn;

		imshow("webcam", cameraFrame);

		//release the memory
		cameraFrame.release();

		if (currentFingerLoops == fingerLoopsThreshold) break;

	}

	//Destroy all windows, close webcam
	destroyWindow("webcam");
	destroyWindow("treshold");
	destroyWindow("threshold noise reduced");
	webcam.release();

	return fingerCountReturn;
}

//the important function to track the hand, the algorithm is described in the report
int HandDetection::trackHand(Mat src, Mat& dest) {

	Rect boundRect;
	int largestObj;
	vector<vector<Point> > contours; 
	vector<vector<Point> > contoursSet(contours.size());
	vector<Vec4i> hierarchy;
	vector<Point> convexHullPoint;
	Point centerP;
	int numObjects = 0;
	double area = 0;
	double maxArea = 0;
	bool handFound = false;


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

			int pos = 0;
			for (int i = 1; i < convexHullPoint.size(); i++) {

				pos = i;
				if (centerP.y >= convexHullPoint[i].y && centerP.y >= convexHullPoint[pos].y) {

					pos = i;
					if (i == 0 || abs(convexHullPoint[i - 1].x - convexHullPoint[i].x) >= 12) {

						fingerCount++;
						line(dest, centerP, convexHullPoint[pos], Scalar(0, 255, 0), 3, 8, 0);
						circle(dest, convexHullPoint[pos], 8, Scalar(255, 0, 0), FILLED);
						pos = i;
					}
				}
			}

			//Limit to 5 fingers
			if (fingerCount > 5) {
				fingerCount = 5;
			}
			
			putText(dest, to_string(fingerCount), printPoint, 1, 5, Scalar(0, 255, 0), 1, 5, false);
		}
	}
	return fingerCount;
}

//Reduces noice in image
void HandDetection::noiseReduction(Mat& frame) {

	for (int i = 0; i < 3; i++) { erode(frame, frame, Mat()); }
	
	for (int i = 0; i < 4; i++) { dilate(frame, frame, Mat()); }

}


HandDetection::~HandDetection()
{
}

