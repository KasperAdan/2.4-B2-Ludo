#pragma once
#include <vector>
#include <stdio.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


class Dobble
{
public:
	Dobble();
	Dobble(int givenWebcamNr);
	~Dobble();
	int findDice();

private:	
};