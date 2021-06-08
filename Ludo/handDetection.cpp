#include "handDetection.h"

#include <iostream>
#define PI 3.14159265

using namespace std;
using namespace cv;

int hmin = 0,	smin = 0,	vmin = 161;
int hmax = 73,	smax = 173,	vmax = 255;


handDetection::handDetection()
{
	VideoCapture vCapture;
	vCapture.open(0);

	namedWindow("Trackbars", (614, 200));
	createTrackbar("Hue min", "Trackbars", &hmin, 179);
	createTrackbar("Hue max", "Trackbars", &hmax, 179);
	createTrackbar("Sat min", "Trackbars", &smin, 255);
	createTrackbar("Sat max", "Trackbars", &smax, 255);
	createTrackbar("Val min", "Trackbars", &vmin, 255);
	createTrackbar("Val max", "Trackbars", &vmax, 255);
	
	while (1) {
		getCommand(vCapture);
		waitKey(1);
	}
}


handDetection::~handDetection()
{
}

string handDetection::getCommand(VideoCapture vCapture) {
	Mat cameraFrame, blurFrame, closedFrame, hsvFrame, thresholdFrame;
	string command = "nothing-new";

	vCapture.read(cameraFrame);
	//switch the RGB to HSV space
	cvtColor(cameraFrame, hsvFrame, COLOR_BGR2HSV);


	//testing in the blue glove on hand
	//need to adjust before the live demo
	inRange(hsvFrame, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdFrame);

	imshow("treshold", thresholdFrame);
	erode(thresholdFrame, thresholdFrame, Mat());
	erode(thresholdFrame, thresholdFrame, Mat());


	dilate(thresholdFrame, thresholdFrame, Mat());
	dilate(thresholdFrame, thresholdFrame, Mat());
	dilate(thresholdFrame, thresholdFrame, Mat());
	dilate(thresholdFrame, thresholdFrame, Mat());

	imshow("threshold dilated", thresholdFrame);

	//Tracking actual hand in restricted background
	//cv::inRange(hsvFrame, Scalar(96, 23, 123), Scalar(256, 100, 256),thresholdFrame);

	//blur image to remove basic imperfections
	medianBlur(thresholdFrame, thresholdFrame, 5);

	//do the morphological image processing
	//closing the frame
	morphologicalImgProc(thresholdFrame);

	//track the hand, put the bounding box around the hand
	//calculate the center point of the hand
	command = trackHand(thresholdFrame, cameraFrame);

	namedWindow("Hand_Gesture_Detection");
	imshow("Hand_Gesture_Detection", cameraFrame);

	//release the memory
	cameraFrame.release();

	return command;

}

//calculate the angle between two points
int handDetection::angleToCenter(const Point& finger, const Point& center) {
	float y_angle = center.y - finger.y; //center = 1;
	float x_angle = finger.x - center.x;// tip =2;
	float theta = atan(y_angle / x_angle);
	int angleFinger = (int)round(theta * 180 / PI);
	return angleFinger;
}

//convert the integer to string
string handDetection::integerToString(int num) {
	stringstream strings;
	strings << num;
	string s = strings.str();
	return s;
}

//morphological Image processing
//Erosion -> dilation -> closing the frame ensure to get the better performance
void handDetection::morphologicalImgProc(Mat& frame) {
	Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, Size(9, 9), Point(5, 5));
	Mat element1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, Size(7, 7), Point(5, 5));
	cv::dilate(frame, frame, element);
	cv::erode(frame, frame, element);
	cv::morphologyEx(frame, frame, MORPH_OPEN, element);
	cv::morphologyEx(frame, frame, MORPH_CLOSE, element);
}

//the important function to track the hand, the algorithm is described in the report
string handDetection::trackHand(Mat src, Mat& dest) {
	//initialization local variables
	Rect boundRect;
	int largestObj;
	vector<vector<Point> > contours; //store all the contours
	vector<vector<Point> > contoursSet(contours.size());//store large contours
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
	for (unsigned int i = 0; i < contours.size(); i++) {
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
			for (int j = 1; j < countHullPoint; j++) {
				pos = j;
				if (centerP.y >= convexHullPoint[j].y && centerP.y >= convexHullPoint[pos].y) {
					pos = j;
					int dist = (centerP.x - convexHullPoint[j].x) ^ 2 + (centerP.y - convexHullPoint[j].y) ^ 2;
					if (abs(convexHullPoint[j - 1].x - convexHullPoint[j].x) < 12) {
						if (dist > maxdist) {
							maxdist = dist;
							pos = j;
						}
					}
					else if (j == 0 || abs(convexHullPoint[j - 1].x - convexHullPoint[j].x) >= 12) {
						fingerPoint.push_back(convexHullPoint[pos]);
						cv::line(dest, centerP, convexHullPoint[pos], Scalar(0, 255, 0), 3, 8, 0);
						circle(dest, convexHullPoint[pos], 8, Scalar(255, 0, 0), FILLED);
						pos = j;
					}
				}
			}

			//get the size the fingers, and calculate the total angle of these fingers
			int countFinger = fingerPoint.size();
			int angle = 0;
			if (countFinger <= 5) {
				for (int x = 0; x < countFinger; x++) {
					angle = angle + abs(angleToCenter(fingerPoint[x], centerP));
				}
			}
			//cout << angle << endl;
//			resultMsg = doAction(angle, countFinger);
			sendResult(resultMsg);
			putText(dest, integerToString(countFinger), printPoint, 1, 5, Scalar(0, 255, 0), 1, 5, false);
		}
	}
	return resultMsg;
}

////send out the result signal
void handDetection::sendResult(String msg) {
	cout << "Command: " << msg << endl;
}