#include "BubbleCounter.h"

#include <iostream>
#include <map>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <set>

/**
 * Algorithm top level
 */

cv::Mat getShapes(std::vector<cv::Mat>& frames);
std::vector<cv::Point> getBrightSpots(cv::Mat& frame, bool adaptive, int dilateIterations);

struct ComponentInfo {
    int bubbleCount;
    std::vector<cv::Mat> components;
    std::vector<std::vector<cv::Point>> componentSpots;

    // Directly from connectedComponents function
    int numLabels;
    cv::Mat labels, stats, centroids;
};

ComponentInfo getComponentInfo(cv::Mat& shapes, std::vector<cv::Point>& brightSpots);
std::vector<cv::Vec3f> getCircles(ComponentInfo& ci);
void showOverlay(cv::Mat& frame, cv::Mat& shapes, ComponentInfo& ci);

std::vector<cv::Vec3f> algorithm(std::vector<cv::Mat>& frames) {
    // Bright spot detection parameters
    bool adaptive = true;
    int dilateIterations = 2;

    cv::Mat shapes = getShapes(frames);
    std::vector<cv::Point> brightSpots = getBrightSpots(frames.back(), adaptive, dilateIterations);
    ComponentInfo ci = getComponentInfo(shapes, brightSpots);
    std::vector<cv::Vec3f> circles = getCircles(ci);

    showOverlay(frames.back(), shapes, ci);

    return circles;
}

/**
 * Algorithm subroutines
 */

cv::Mat getShapes(std::vector<cv::Mat>& frames) {
    cv::Mat diff, blur, thresh;

    auto subtractor = cv::createBackgroundSubtractorKNN();
    for (cv::Mat f : frames) subtractor->apply(f, diff);
    cv::GaussianBlur(diff, blur, cv::Size(5, 5), 0);
    cv::threshold(blur, thresh, 10, 255, cv::THRESH_BINARY);

    cv::imshow("Diff", diff);
    cv::imshow("Mask", thresh);
    cv::waitKey(5);

    return thresh;
}

std::vector<cv::Point> getBrightSpots(cv::Mat& frame, bool adaptive, int dilateIterations) {
    cv::Mat blur, thresh, dilated, precontour;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> spots;

    if (adaptive)
        cv::adaptiveThreshold(frame, thresh, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, -10);
    else {
        cv::GaussianBlur(frame, blur, cv::Size(5, 5), 0);
        cv::threshold(blur, thresh, 100, 255, cv::THRESH_BINARY);
    }
    if (dilateIterations == 0)
        precontour = thresh;
    else {
        cv::dilate(thresh, dilated, cv::Mat(), cv::Point(-1, -1), dilateIterations);
        precontour = dilated;
    }

    cv::findContours(precontour, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (auto contour : contours) {
        auto br = cv::boundingRect(contour);
        spots.push_back(cv::Point(br.x + br.width / 2, br.y + br.height / 2));
    }

    cv::imshow("spots precontour", precontour);
    cv::waitKey(5);

    return spots;
}

ComponentInfo getComponentInfo(cv::Mat& shapes, std::vector<cv::Point>& brightSpots) {
    ComponentInfo ci;

    ci.numLabels = cv::connectedComponentsWithStats(shapes, ci.labels, ci.stats, ci.centroids);
    ci.bubbleCount = ci.numLabels - 1;
    ci.componentSpots = std::vector<std::vector<cv::Point>>(ci.numLabels);

    // Components with n >= 2 spots get n bubbles, otherwise 1
    for (cv::Point spot : brightSpots) {
        int label = ci.labels.at<int>(spot);
        if (label <= 0 || label >= ci.numLabels) continue;
        if (ci.componentSpots[label].size() > 0) ci.bubbleCount++;
        ci.componentSpots[label].push_back(spot);
    }

    for (int label = 0; label < ci.numLabels; label++) {
        cv::Mat mat(shapes.size(), shapes.type());
        mat.setTo(cv::Scalar(0));
        ci.components.push_back(mat.clone());
    }

    for (int y = 0; y < shapes.rows; y++) {
        for (int x = 0; x < shapes.cols; x++) {
            int label = ci.labels.at<int>(y, x);
            ci.components[label].at<uint8_t>(y, x) = 255;
        }
    }

    return ci;
}

void addSimpleBubbleCircle(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label);
void addComplexBubbleCircles(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label);

std::vector<cv::Vec3f> getCircles(ComponentInfo& ci) {
    std::vector<cv::Vec3f> circles;

    //Sort components large to small
    std::vector<int> sortedLabels;
    for (int label = 1; label < ci.numLabels; label++)
        sortedLabels.push_back(label);
    auto comp = [ci](int l1, int l2) {
        int area1 = ci.stats.at<int>(l1, cv::CC_STAT_AREA);
        int area2 = ci.stats.at<int>(l2, cv::CC_STAT_AREA);
        return area1 > area2;
    };
    std::sort(sortedLabels.begin(), sortedLabels.end(), comp);

    for (int label : sortedLabels) {
        if (ci.componentSpots[label].size() < 2)
            addSimpleBubbleCircle(circles, ci, label);
        else
            addComplexBubbleCircles(circles, ci, label);
    }

    return circles;
}

int searchRadius(cv::Mat& background, cv::Point center, int maxRadius) {
    // Perform binary search
    int lower = 0, upper = maxRadius;
    cv::Mat blank(background.size(), background.type()), mask, andmat;
    blank.setTo(cv::Scalar(0));

    while (lower != upper) {
        int radius = std::ceil((lower + upper) / 2.0);
        cv::Mat mask = blank.clone();
        cv::circle(mask, center, radius, cv::Scalar(255), -1);
        cv::bitwise_and(mask, background, andmat);
        if (cv::countNonZero(andmat) > 0)
            upper = radius - 1;
        else
            lower = radius;
    }

    cv::circle(background, center, lower, cv::Scalar(255), -1);
    return lower;
}

void addSimpleBubbleCircle(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label) {
    double area = ci.stats.at<int>(label, cv::CC_STAT_AREA);
    double pi = std::acos(-1.0);
    double radius = std::sqrt(area / pi);

    int cx = ci.centroids.at<double>(label, 0);
    int cy = ci.centroids.at<double>(label, 1);

    //Squeeze radius to conform to existing circles
    int sqRad = searchRadius(ci.components[0], cv::Point(cx, cy), radius);
    if (sqRad == 0) return;

    circles.push_back(cv::Vec3f(cx, cy, sqRad));
}

void addComplexBubbleCircles(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label) {
    cv::Mat background = ci.components[0];

    int bbw = ci.stats.at<int>(label, cv::CC_STAT_WIDTH);
    int bbh = ci.stats.at<int>(label, cv::CC_STAT_WIDTH);
    int maxRadius = std::min(bbw, bbh) / 2;

    for (cv::Point spot : ci.componentSpots[label]) {
        int radius = searchRadius(background, spot, maxRadius);
        if (radius == 0) continue;
        circles.push_back(cv::Vec3f(spot.x, spot.y, radius));
    }
}

cv::Mat upscale(cv::Mat& frame) {
    cv::Mat upscaled;
    int newWidth = 512;
    int newHeight = frame.rows * newWidth / frame.cols;
    cv::resize(frame, upscaled, cv::Size(newWidth, newHeight));
    return upscaled;
}

void showOverlay(cv::Mat& frame, cv::Mat& shapes, ComponentInfo& ci) {
    cv::Mat overlay, canny;
    cv::cvtColor(frame, overlay, cv::COLOR_GRAY2BGR);

    for (int label = 1; label < ci.numLabels; label++)
        for (auto spot : ci.componentSpots[label]) cv::circle(overlay, spot, 1, cv::Scalar(0, 0, 255));

    const double sigma = 0.33;
    uint8_t mean = cv::mean(shapes)[0];
    double lower = std::max(0.0, (1 - sigma) * mean);
    double upper = std::min(255.0, (1 + sigma) * mean);
    cv::Canny(shapes, canny, lower, upper);
    overlay.setTo(cv::Scalar(255, 0, 0), canny);

    cv::imshow("Overlay", upscale(overlay));
    cv::waitKey(5);
}

/**
 * This part reads video and calls the algorithm
 */

void analyze(std::vector<cv::Mat>& frames, cv::Size newSize) {
    std::cout << "analyze called";
    cv::imshow("Frame", frames.back());
    cv::waitKey(5);

    // Resizing
    std::vector<cv::Mat> resizedFrames;
    int i = 1;
    for (cv::Mat frame : frames) {
        cv::Mat resized;
        cv::resize(frame, resized, newSize, 0, 0, cv::INTER_AREA);
        cv::cvtColor(resized, resized, cv::COLOR_BGR2GRAY);
        resizedFrames.push_back(resized);
        cv::imshow(std::string("Resized " + std::to_string(i)), resized);
        cv::waitKey(5);
        i++;
    }

    // Algorithm
    auto circles = algorithm(resizedFrames);

    // Display results
    cv::Mat foundCircles;
    cv::cvtColor(resizedFrames.back(), foundCircles, cv::COLOR_GRAY2BGR);
    std::cout << ", found " << circles.size() << " circles" << std::endl;
    for (cv::Vec3f circle : circles) {
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::circle(foundCircles, center, radius, cv::Scalar(0, 0, 255));
    }

    cv::imshow("Found circles", upscale(foundCircles));
    cv::waitKey(100000);
}

int getWidth(int frameCount) {
    const std::set<int> validFrameCounts = {546, 2184, 8736};  // From Kodak Motion Corder manual, page 3.5
    const std::map<int, int> widthMap = {{546, 512}, {2184, 256}, {8736, 128}};
    const int slop = 5;  // Allow for a few skipped frames etc... video encoding is fucky lol

    for (int valid : validFrameCounts)
        if (std::abs(frameCount - valid) <= slop) return widthMap.at(valid);

    std::cerr << "Invalid frame count! Cannot pick frame width." << std::endl;
    return -1;
}

void BubbleCounter::run(cv::VideoCapture& videoIn) {
    const int analyzeTotal = 10;

    int frameCount = videoIn.get(cv::CAP_PROP_FRAME_COUNT);
    if (frameCount == 0) {
        std::cerr << "Invalid frame count of 0! Please check that the video file is not corrupted." << std::endl;
        std::cerr << "大変！ビデオで０フレームがあります。bubble-countには、壊れないファイルをあげてください。" << std::endl;
        return;
    }

    int interval = frameCount / analyzeTotal;
    int frameCounter = 1, analyzeCounter = 0;

    std::cout << "Interval is " << interval << " frames" << std::endl;

    const int frameBufferSize = 15;
    std::vector<cv::Mat> frames;

    cv::Mat frame;
    videoIn.read(frame);

    int newWidth = getWidth(frameCount);
    int newHeight = frame.rows * newWidth / frame.cols;
    cv::Size newSize(newWidth, newHeight);

    for (; !frame.empty() && analyzeCounter < analyzeTotal; videoIn.read(frame)) {
        if (frames.size() == frameBufferSize) frames.erase(frames.begin());
        frames.push_back(frame.clone());

        if (frameCounter % interval == 0) {
            std::cout << "Frame " << frameCounter << ", ";
            analyze(frames, newSize);
            analyzeCounter++;
        }
        frameCounter++;
    }
}
