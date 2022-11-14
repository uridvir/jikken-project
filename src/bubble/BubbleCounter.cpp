#include "BubbleCounter.h"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <map>

void analyze(cv::Mat frame, cv::Size newSize){
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
    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(resized, circles, cv::HOUGH_GRADIENT, 1,
        10, //minimum distance between bubbles (pixels)
        300, 0.95, //parameters
        1, 15 //Min and max bubble size (pixels)
    );
    if (circles.size() == 0){
        std::cout << ", found no circles" << std::endl;
        return;
    }

    std::cout << ", found " << circles.size() << " circles" << std::endl;
    for (cv::Vec3f circle : circles){
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::circle(foundCircles, center, radius, cv::Scalar(0, 0, 255));
    }
    cv::imshow("Found circles", foundCircles);
    cv::waitKey(1000);
}

void BubbleCounter::run(cv::VideoCapture* videoIn){
    const int analyzeTotal = 10;

    //TODO: FIX THIS
    //int totalFrames = videoIn->get(cv::CAP_PROP_FRAME_COUNT);
    int totalFrames = 2184; //Hack because frame count isn't working right now

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
        if (frameCounter % interval == 0){
            std::cout << "Frame " << frameCounter << ", ";
            analyze(frame, newSize);
            analyzeCounter++;
        } 
        frameCounter++;
    }
}
