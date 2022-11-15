#include "BubbleCounter.h"

#include <iostream>
#include <map>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void showOverlay(cv::Mat frame, cv::Mat mask){
    cv::Mat overlay;
    cv::cvtColor(frame, overlay, cv::COLOR_GRAY2BGR);
    overlay.setTo(cv::Scalar(255, 0, 0), mask);

    cv::imshow("Overlay", overlay);
    cv::waitKey(5);
}

std::vector<cv::Vec3f> method1(std::vector<cv::Mat> frames){
    cv::Mat thresholded, frame = frames.back();
    std::vector<cv::Vec3f> circles;

    cv::adaptiveThreshold(frame, thresholded, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C, //Method
        cv::THRESH_BINARY_INV, //Threshold type
        9, 5 //Block size and constant
    );
    cv::imshow("Thresholded", thresholded);
    cv::waitKey(5);

    showOverlay(frame, thresholded);

    cv::HoughCircles(frame, circles, cv::HOUGH_GRADIENT, 1,
        10, //minimum distance between bubbles (pixels)
        300, 0.95, //parameters
        1, 10 //Min and max bubble size (pixels)
    );

    return circles;
}

std::vector<cv::Vec3f> method2(std::vector<cv::Mat> frames){
    cv::Mat thresholded, frame = frames.back();
    std::vector<cv::Vec3f> circles;

    uint8_t meanBrightness = cv::mean(frame)[0];
    cv::threshold(frame, thresholded,
        meanBrightness * 1.00, //The threshold
        0, //Max value
        cv::THRESH_TOZERO //Threshold type
    );

    cv::imshow("Thresholded", thresholded);
    cv::waitKey(5);

    return circles;
}

std::vector<cv::Vec3f> method3(std::vector<cv::Mat> frames){
    cv::Mat canny, frame = frames.back();
    std::vector<cv::Vec3f> circles;

    uint8_t mean = cv::mean(frame)[0];
    double sigma = 0.33;

    double lower = std::max(0.0, (1 - sigma) * mean);
    double upper = std::min(255.0, (1 + sigma) * mean);
    cv::Canny(frame, canny, lower, upper);

    cv::imshow("Canny", canny);
    cv::waitKey(5);

    showOverlay(frame, canny);

    return circles;
}

std::vector<cv::Vec3f> method4(std::vector<cv::Mat> frames){
    cv::Mat diff;
    std::vector<cv::Vec3f> circles;
    cv::absdiff(frames[1], frames[0], diff);

    cv::imshow("Frame diff", diff);
    cv::waitKey(5);

    return circles;
}

void analyze(std::vector<cv::Mat> frames, cv::Size newSize) {
    std::cout << "analyze called";
    cv::imshow("Frame", frames.back());
    cv::waitKey(5);

    std::vector<cv::Mat> resizedFrames;
    int i = 1;
    for (cv::Mat frame : frames) {
        cv::Mat resized;
        cv::resize(frame, resized, newSize, 0, 0, cv::INTER_AREA);
        cv::cvtColor(resized, resized, cv::COLOR_BGR2GRAY);
        resizedFrames.push_back(resized);
        char str[50];
        sprintf(str, "Resized %d", i);
        cv::imshow(str, resized);
        cv::waitKey(5);
        i++;
    }

    cv::Mat foundCircles;
    cv::cvtColor(resizedFrames.back(), foundCircles, cv::COLOR_GRAY2BGR);
    std::vector<cv::Vec3f> circles = method4(resizedFrames); //Change which method is used here

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
    std::vector<cv::Mat> frames;
    const int frameCount = 2;
    videoIn->read(frame);
    const std::map<int, int> widthMap = {{546, 512}, {2184, 256}, {8736, 128}};
    int newWidth = widthMap.at(totalFrames);
    int newHeight = frame.rows * newWidth / frame.cols;
    cv::Size newSize(newWidth, newHeight);

    for (; !frame.empty() && analyzeCounter < analyzeTotal; videoIn->read(frame)) {
        if (frames.size() == frameCount)
            frames.erase(frames.begin());
        frames.push_back(frame.clone());
        
        if (frameCounter % interval == 0) {
            std::cout << "Frame " << frameCounter << ", ";
            analyze(frames, newSize);
            analyzeCounter++;
        }
        frameCounter++;
    }
}
