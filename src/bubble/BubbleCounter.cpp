#include "BubbleCounter.h"

#include <iostream>
#include <map>
#include <set>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

void showOverlay(cv::Mat frame, cv::Mat mask, char* name = "Overlay", bool ungray = true){
    cv::Mat overlay;
    if (ungray) cv::cvtColor(frame, overlay, cv::COLOR_GRAY2BGR);
    else overlay = frame.clone();
    overlay.setTo(cv::Scalar(255, 0, 0), mask);

    cv::imshow(name, overlay);
    cv::waitKey(5);
}

cv::Mat autoCanny(cv::Mat frame, double sigma = 0.33){
    cv::Mat canny;
    uint8_t mean = cv::mean(frame)[0];

    double lower = std::max(0.0, (1 - sigma) * mean);
    double upper = std::min(255.0, (1 + sigma) * mean);
    cv::Canny(frame, canny, lower, upper);

    return canny;
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

    canny = autoCanny(frame);

    cv::imshow("Canny", canny);
    cv::waitKey(5);

    showOverlay(frame, canny);

    return circles;
}

std::vector<cv::Vec3f> method4(std::vector<cv::Mat> frames){
    cv::Mat diff, frame = frames[frames.size() - 1], prevFrame = frames[frames.size() - 2];
    std::vector<cv::Vec3f> circles;

    cv::absdiff(frame, prevFrame, diff);

    cv::imshow("Frame diff", diff);
    cv::waitKey(5);

    cv::Mat blur, thresh, dilated;
    cv::GaussianBlur(diff, blur, cv::Size(5, 5), 0);
    cv::threshold(blur, thresh, 30, 255, cv::THRESH_BINARY);
    cv::dilate(thresh, dilated, 
        cv::Mat(), cv::Point(-1, -1), //Defaults
        3 //Iterations
    );

    cv::imshow("Dilated", dilated);
    cv::waitKey(5);
    showOverlay(frames[1], dilated);

    return circles;
}

std::vector<cv::Vec3f> method5(std::vector<cv::Mat> frames){
    std::vector<cv::Vec3f> circles;
    cv::Mat frame = frames.back();
    cv::Mat mask, blur, thresh, canny;
    
    //Get mask and mask post, make canny
    auto subtractor = cv::createBackgroundSubtractorKNN();
    for (cv::Mat f : frames) subtractor->apply(f, mask);

    cv::imshow("Mask", mask);
    cv::waitKey(5);

    cv::GaussianBlur(mask, blur, cv::Size(5, 5), 0);
    cv::threshold(blur, thresh, 0, 255, cv::THRESH_BINARY);
    canny = autoCanny(thresh);

    cv::imshow("Mask post", thresh);
    cv::waitKey(5);

    //Find bright spots
    cv::Mat thresh2, dilated2;
    cv::adaptiveThreshold(frame, thresh2, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, -10);
    cv::dilate(thresh2, dilated2, cv::Mat(), cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dilated2, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> spots;
    for (auto contour : contours){
        auto br = cv::boundingRect(contour);
        spots.push_back(cv::Point(br.x + br.width/2, br.y + br.height/2));
    }

    //Count components in mask and update bubble count
    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(thresh, labels, stats, centroids);
    int numBubbles = numLabels - 1;
    std::vector<std::vector<cv::Point> > compSpots(numLabels);
    std::vector<cv::Point> displaySpots;

    for (cv::Point spot : spots){
        //Check if there is a component at the spot
        int label = labels.at<int>(spot);
        //Ignore spots not in a component
        if (label <= 0 || label >= numLabels) continue; 
        //Components get new bubbles for every spot after the first
        if (compSpots[label].size() > 0) numBubbles++;
        //Update component spot list
        compSpots[label].push_back(spot);
    }
    for (int label = 1; label < numLabels; label++){ //Update display vars
        auto spotList = compSpots[label];
        if (spotList.size() >= 2){
            for (auto spot : spotList){
                displaySpots.push_back(spot);
                circles.push_back(cv::Vec3f(spot.x, spot.y, 3));
            }
        }
        else {
            int cx = centroids.at<double>(label, 0);
            int cy = centroids.at<double>(label, 1);
            circles.push_back(cv::Vec3f(cx, cy, 3));
        }
    }

    //Draw overlay with spots
    std::cout << ", found " << numBubbles << " bubbles";
    cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
    for (auto spot : displaySpots) cv::circle(frame, spot, 1, cv::Scalar(0, 0, 255));
    showOverlay(frame, canny, "Overlay", false);

    //for (int i = 0; i < numBubbles; i++)
    //    circles.push_back(cv::Vec3f(0,0,0));
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
    std::vector<cv::Vec3f> circles = method5(resizedFrames); //Change which method is used here

    std::cout << ", found " << circles.size() << " circles" << std::endl;
    for (cv::Vec3f circle : circles){
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::circle(foundCircles, center, radius, cv::Scalar(0, 0, 255));
    }
    cv::imshow("Found circles", foundCircles);
    cv::waitKey(100000);
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
    const int frameCount = 15;
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
