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

	int getCommand(VideoCapture vCapture);

private:
	void noiseReduction(Mat& frame);
	int trackHand(Mat src, Mat& dest);

};