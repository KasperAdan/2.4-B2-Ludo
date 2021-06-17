#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class HandDetection
{
public:
	HandDetection(int handDetectionWebcamNr);
	~HandDetection();

	int findFingers();

private:
	void noiseReduction(Mat& frame);
	int trackHand(Mat src, Mat& dest);

};