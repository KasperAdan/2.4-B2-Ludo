#include <vector>
#include "dobble.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int webcamNr = 0;
int loop_equal = 0;
int previous_equal = 0;
int full_zero_check = 0;
int TOTAL_LOOPS = 20;
int lastDiceCounts[6] = { 0, 0, 0, 0, 0, 0 };
int diceCounts[6] = { 0, 0, 0, 0, 0, 0 };

Dobble::Dobble(int givenWebcamNr)
{
    webcamNr = givenWebcamNr;
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
            if ((rect.size.area() > 3500) && (rect.size.area() < 5000)) {

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
        int diceCounts[6] = { 0, 0, 0, 0, 0, 0 };
        int totalDiceDots = 0;
        for (int i = 0; i < diceRects.size(); i++) {

            totalDiceDots = 0;

            // Extract die image
            Mat rotation, rotated, cropped;
            RotatedRect rect = diceRects[i];
            rotation = getRotationMatrix2D(rect.center, rect.angle, 1.0);
            warpAffine(cannyImage, rotated, rotation, cannyImage.size(), INTER_CUBIC);
            getRectSubPix(rotated, Size(rect.size.width - 10, rect.size.height - 10), rect.center, cropped);

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
                diceCounts[dotsRects.size() - 1]++;

            }
        }

        previous_equal = 0;
        full_zero_check = 0;

        //Array equal checl
        for (int i = 0; i < 6; i++) {

            //Look if the two arrays are equal
            if (lastDiceCounts[i] == diceCounts[i]) {
                previous_equal++;
            }
            else previous_equal = 0;

            //If equal arrays
            if (previous_equal == 6) {
                loop_equal++;
            }

            //Checks if its is not an empty array
            if (diceCounts[i] == 0) {
                full_zero_check++;
            }
            else full_zero_check = 0;

            //If empty array, equal assertion fails
            if (full_zero_check == 6) {
                loop_equal = 0;
            }
        }
        //If the total loops is reached, break the while loop
        if (loop_equal == TOTAL_LOOPS) {
            break;
        }

        //If the arrays are not equal, the loop was not equal with the previous
        if (previous_equal != 6) {
            loop_equal = 0;
        }

        //Display dot count
        for (int i = 0; i < 6; i++) {

            int count = 0;
            count += diceCounts[i];
            sprintf_s(text, "%d: %d", (i + 1), count);
            putText(image, text, Point(20, 55 + 25 * i), FONT_HERSHEY_DUPLEX, 0.8, Scalar(0, 255, 0), 1, LINE_AA);
            lastDiceCounts[i] = diceCounts[i];
        }

        imshow("Canny", cannyImage);
        imshow("Final Image", image);

        waitKey(1);
    }

    webcam.release();
    
    for (int i = 0; i < 6; i++) {
        if (diceCounts[i] == 1) {
            return i + 1;
        }
    }
    return 0;
}

Dobble::~Dobble()
{
}
