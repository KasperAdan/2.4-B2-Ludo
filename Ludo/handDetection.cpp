#include "handDetection.h"

#include <iostream>

using namespace std;
using namespace cv;

int hmin = 0,	smin = 0,	vmin = 120;
int hmax = 35,	smax = 150,	vmax = 250;

int fingerCount;
int handDetectionWebcam;


HandDetection::HandDetection(int handDetectionWebcamNr)
{
	handDetectionWebcam = handDetectionWebcamNr;

	namedWindow("Trackbars", (614, 200));
	createTrackbar("Hue min", "Trackbars", &hmin, 179);
	createTrackbar("Hue max", "Trackbars", &hmax, 179);
	createTrackbar("Sat min", "Trackbars", &smin, 255);
	createTrackbar("Sat max", "Trackbars", &smax, 255);
	createTrackbar("Val min", "Trackbars", &vmin, 255);
	createTrackbar("Val max", "Trackbars", &vmax, 255);
	
}

int HandDetection::findFingers() {

	VideoCapture webcam;
	webcam.open(handDetectionWebcam);

	int fingerCountReturn = 0;
	int previousFingerCount = 0;
	int fingerLoopsThreshold = 15;
	int currentFingerLoops = 0;

	while (1) {
		Mat cameraFrame, hsvFrame, thresholdFrame;
		webcam.read(cameraFrame);

		//switch the RGB to HSV space
		cvtColor(cameraFrame, hsvFrame, COLOR_BGR2HSV);

		//Adjust colors to find hand
		inRange(hsvFrame, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdFrame);

		//Erodes and Dilates for noise reduction
		//imshow("treshold", thresholdFrame);
		noiseReduction(thresholdFrame);
		//imshow("threshold noise reduced", thresholdFrame);

		//track the hand, put the bounding box around the hand
		//calculate the center point of the hand
		fingerCountReturn = trackHand(thresholdFrame, cameraFrame);

		if (fingerCountReturn == previousFingerCount && fingerCountReturn != 0) {
			currentFingerLoops++;
		}
		else currentFingerLoops = 0;

		previousFingerCount = fingerCountReturn;

		imshow("Hand Detection", cameraFrame);

		//release the memory
		cameraFrame.release();

		if (currentFingerLoops == fingerLoopsThreshold) break;


		waitKey(1);
	}

	//Destroy all windows, close webcam
	/*destroyWindow("Final Image");
	destroyWindow("treshold");
	destroyWindow("threshold noise reduced");*/
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
	vector<Point> foundFingers;
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

		//draw the boundary of the object //Commented for Demo
		//drawContours(dest, contours, largestObj, Scalar(0, 0, 255), 3, 8, hierarchy);
		//find the convex points for the largest object which is hand
		convexHull(contours[largestObj], convexHullPoint, true, true);

		//use moment method to find the center point
		Moments moment = moments(Mat(contours[largestObj]), true);
		int centerX = moment.m10 / moment.m00;
		int centerY = moment.m01 / moment.m00;
		Point centerPoint(centerX, centerY);
		centerP = centerPoint;
		Point printPoint(centerX, centerY + 50);
		Point printPoint1(boundRect.x, boundRect.y);
		circle(dest, centerPoint, 8, Scalar(255, 0, 0), FILLED);

		//put the BoundingBox in the contour region //Commented for Demo
		//rectangle(dest, boundRect, Scalar(0, 0, 255), 2, 8, 0);

		if (handFound) {
			bool validFinger = false;
			fingerCount = 0;
			int maxdist = 0;

			int pos = 0;
			for (int i = 1; i < convexHullPoint.size(); i++) {
				
				pos = i;
				//If finger is above hand centre, prevents wrist contours
				if (centerP.y + 10 >= convexHullPoint[i].y && centerP.y + 10 >= convexHullPoint[pos].y) {

					pos = i;
					//If next finger is at least so far away, prevents duplicate contours on same finger
					if (i == 0 || abs(convexHullPoint[i - 1].x - convexHullPoint[i].x) >= 25) {
						validFinger = true;
						foundFingers.push_back(convexHullPoint[i]);
						pos = i;
					}
					else if (convexHullPoint[i - 1].y > convexHullPoint[i].y && validFinger) {

						foundFingers.pop_back();
						foundFingers.push_back(convexHullPoint[i]);

					}
					else validFinger = false;
				}
				
			}
			for(Point point : foundFingers) {
				line(dest, centerP, point, Scalar(0, 255, 0), 3, 81, 0);
				circle(dest, point, 8, Scalar(255, 0, 0), FILLED);
			}
			putText(dest, to_string(foundFingers.size()), printPoint, 1, 5, Scalar(255, 0, 0), 5, 5, false);
		}
	}
	
	return foundFingers.size() < 4 ? foundFingers.size() : 4;
}

//Reduces noice in image
void HandDetection::noiseReduction(Mat& frame) {

	for (int i = 0; i < 3; i++) { erode(frame, frame, Mat()); }
	
	for (int i = 0; i < 5; i++) { dilate(frame, frame, Mat()); }

}


HandDetection::~HandDetection()
{
}

