#include "BubbleCounter.h"

#include <iostream>
#include <map>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

std::vector<cv::Vec3f> method1(cv::Mat frame){
    cv::Mat thresholded;
    std::vector<cv::Vec3f> circles;

    cv::adaptiveThreshold(frame, thresholded, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C, //Method
        cv::THRESH_BINARY_INV, //Threshold type
        7, 7 //Block size and constant
    );
    cv::imshow("Thresholded", thresholded);
    cv::waitKey(5);

    cv::HoughCircles(frame, circles, cv::HOUGH_GRADIENT, 1,
        10, //minimum distance between bubbles (pixels)
        300, 0.95, //parameters
        1, 10 //Min and max bubble size (pixels)
    );

    return circles;
}

/*

//Settings for blob detector
cv::SimpleBlobDetector::Params params;
params.filterByColor = true;
params.blobColor = 0;

params.minThreshold = 50;
params.maxThreshold = 255;

params.minDistBetweenBlobs = 5;

params.filterByArea = true;
params.minArea = 5;
params.maxArea = 300;

params.filterByConvexity = true;
params.minConvexity = 0;

params.filterByCircularity = true;
params.minCircularity = 0.1;

params.filterByInertia = false;

*/

void analyze(cv::Mat frame, cv::Size newSize) {
    std::cout << "analyze called";
    cv::imshow("Frame", frame);
    cv::waitKey(5);

    cv::Mat resized;
    cv::resize(frame, resized, newSize, 0, 0, cv::INTER_AREA);
    cv::cvtColor(resized, resized, cv::COLOR_BGR2GRAY);
    cv::imshow("Resized", resized);
    cv::waitKey(5);

    cv::Mat foundCircles;
    cv::cvtColor(resized, foundCircles, cv::COLOR_GRAY2BGR);
    std::vector<cv::Vec3f> circles = method1(resized);

    std::cout << ", found " << circles.size() << " circles" << std::endl;
    for (cv::Vec3f circle : circles){
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::circle(foundCircles, center, radius, cv::Scalar(0, 0, 255));
    }
    cv::imshow("Found circles", foundCircles);
    cv::waitKey(10000);
}

void BubbleCounter::run(cv::VideoCapture* videoIn) {
    const int analyzeTotal = 10;

    // TODO: FIX THIS
    // int totalFrames = videoIn->get(cv::CAP_PROP_FRAME_COUNT);
    int totalFrames = 2184;  // Hack because frame count isn't working right now

    int interval = totalFrames / analyzeTotal;
    int frameCounter = 1, analyzeCounter = 0;

    std::cout << "Interval is " << interval << " frames" << std::endl;

    cv::Mat frame;
    videoIn->read(frame);
    const std::map<int, int> widthMap = {{546, 512}, {2184, 256}, {8736, 128}};
    int newWidth = widthMap.at(totalFrames);
    int newHeight = frame.rows * newWidth / frame.cols;
    cv::Size newSize(newWidth, newHeight);

    for (; !frame.empty() && analyzeCounter < analyzeTotal; videoIn->read(frame)) {
        if (frameCounter % interval == 0) {
            std::cout << "Frame " << frameCounter << ", ";
            analyze(frame, newSize);
            analyzeCounter++;
        }
        frameCounter++;
    }
}
