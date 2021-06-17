#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class HandDetection
{
public:
	HandDetection();
	~HandDetection();

	std::string getCommand(VideoCapture vCapture);

private:
	void noiseReduction(Mat& frame);
	std::string integerToString(int num);
	int angleToCenter(const Point& v1, const Point& v2);
	//	string doAction(int totalAngleOfFinger, int fingerSize);
	void sendResult(string msg);
	string trackHand(Mat src, Mat& dest);

};