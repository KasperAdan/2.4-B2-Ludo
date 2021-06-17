#include <vector>
#include "dobble.h"
#include <stdio.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int webcamNr;
int loop_equal;
int const TOTAL_LOOPS = 20;
vector<int> diceCountsV;
vector<int> lastDiceCountsV;

Dobble::Dobble(int givenWebcamNr)
{
    webcamNr = givenWebcamNr;
    loop_equal = 0;
}

int Dobble::findDice() {

    VideoCapture webcam;
    webcam.open(webcamNr);

    while (1) {
        Mat image, grayImage, cannyImage;
        webcam.read(image);

        //Converts image to grey, and Canny image
        cvtColor(image, grayImage, COLOR_BGR2GRAY);
        blur(grayImage, grayImage, Size(3, 3));
        threshold(grayImage, grayImage, 170, 255, THRESH_BINARY);
        Canny(grayImage, cannyImage, 80, 230);

        //Finds contours in the image
        vector<vector<Point>> diceContours;
        vector<Vec4i> hierarchy;
        findContours(cannyImage, diceContours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        vector<RotatedRect> diceRects;

        for (int i = 0; i < diceContours.size(); i++) {
            // For each contour, search the minimum area rectangle
            RotatedRect rect = minAreaRect(diceContours[i]);

            //Values for webcam, can differ from webcam
            if ((rect.size.area() > 500) && (rect.size.area() < 5000)) {

                // Check if it's a duplicate rectangle
                bool process = true;
                for (int j = 0; j < diceRects.size(); j++) {

                    //If the centre of the rectangles are < 10 distance from each other, they are counted as duplicate
                    float dist = norm(rect.center - diceRects[j].center);
                    if (dist < 10) {

                        process = false;
                        break;
                    }
                }

                if (process) {

                    diceRects.push_back(rect);
                    // Draw square over original image
                    Point2f points[4];
                    rect.points(points);
                    for (int j = 0; j < 4; j++) {

                        line(image, points[j], points[(j + 1) % 4], Scalar(0, 0, 255), 2, LINE_AA);
                    }
                }
            }
        }

        // Write dice count on screen
        char text[32];
        sprintf_s(text, "Dice: %d", (int)diceRects.size());
        putText(image, text, Point(20, 30), FONT_HERSHEY_DUPLEX, 0.8, Scalar(0, 255, 0), 1, LINE_AA);

        // Counting dots of each die
        diceCountsV.clear();
        int totalDiceDots = 0;
        
        for (int i = 0; i < diceRects.size(); i++) {

            totalDiceDots = 0;

            // Extract die image
            Mat rotation, rotated, cropped;
            RotatedRect rect = diceRects[i];
            rotation = getRotationMatrix2D(rect.center, rect.angle, 1.0);
            warpAffine(cannyImage, rotated, rotation, cannyImage.size(), INTER_CUBIC);
            getRectSubPix(rotated, Size(rect.size.width, rect.size.height), rect.center, cropped);

            // Find contours
            vector<vector<Point>> dieContours;
            vector<Vec4i> dieHierarchy;
            threshold(cropped, cropped, 64, 255, THRESH_BINARY);
            findContours(cropped, dieContours, dieHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

            // Find and filter minimum area rects
            vector<RotatedRect> dotsRects;

            for (int i = 0; i < dieContours.size(); i++) {

                // For each contour, try to fit a rectangle
                RotatedRect dotRect = minAreaRect(dieContours[i]);

                // Process only rectangles that are almost square and of the right size.
                if ((dotRect.size.area() > 10) && (dotRect.size.area() < 200)) {

                    // Check if it's a duplicate rect
                    bool valid = true;
                    for (int j = 0; j < dotsRects.size(); j++) {
                        float dist = norm(dotRect.center - dotsRects[j].center);
                        if (dist < 10) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid) {
                        dotsRects.push_back(dotRect);
                    }
                }
            }

            // Save dots count
            if (dotsRects.size() >= 1 && dotsRects.size() <= 6) {
                diceCountsV.push_back(dotsRects.size());
            }
        }

        if (diceCountsV == lastDiceCountsV && diceCountsV.size() != 0) loop_equal++;
        else loop_equal = 0;

        //If the total loops is reached, break the while loop
        if (loop_equal >= TOTAL_LOOPS) {
            loop_equal = 0;
            break;
        }

        //We need to sort the Vector so we can print it in order on sreen
        sort(diceCountsV.begin(), diceCountsV.end());
        int vectorIndex = 0;

        //Display dot count
        for (int i = 0; i < 6; i++) {

            int count = 0;

            //If the value in the Vector equals I, we need to print the value
            //This ignores the values we dont have
            if (vectorIndex < diceCountsV.size() && diceCountsV.at(vectorIndex) - 1 == i) {
                count++;
                vectorIndex++;
            }

            sprintf_s(text, "%d: %d", (i + 1), count);
            putText(image, text, Point(20, 55 + 25 * i), FONT_HERSHEY_DUPLEX, 0.8, Scalar(0, 255, 0), 1, LINE_AA);
            
        }

        //Remember the last dice roll
        lastDiceCountsV = diceCountsV;

        imshow("Final Image", image);

        waitKey(1);
    }

    destroyWindow("Final Image");
    webcam.release();
    
    return diceCountsV.at(0);
}

Dobble::~Dobble()
{
}
