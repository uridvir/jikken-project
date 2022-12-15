#include "BubbleAlgorithm.h"

#include <wx/filefn.h>

#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <set>

#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

/**
 * Front-end of BubbleAlgorithm
 */

struct AlgorithmResult {
    int bubbleCount;
    std::vector<std::string> pictureNames;
    std::map<std::string, cv::Mat> pictures;
};

// Parameters

int analysisCount;
int frameBundleSize;
int shapesBlur;
int shapesBrightnessCutoff;
int brightSpotsBlur;
bool brightSpotsAdaptive;
int brightSpotsAdaptiveC;
int brightSpotsNonAdaptiveBrightnessCutoff;
int brightSpotsDilationCount;

// Forward declaring the algorithm
std::vector<cv::Vec3f> algorithm(const std::vector<cv::Mat>& frames, AlgorithmResult& moreData);

void BubbleAlgorithm::run(std::string filename) {
    cv::VideoCapture videoIn(filename);

    size_t lastDot = filename.find_last_of(".");
    std::string folderPath = filename.substr(0, lastDot);
    wxMkDir(folderPath);

    std::ofstream csv(folderPath + "/data.csv");
    csv << "Analysis #"
        << ","
        << "Bubble count"
        << "," << std::endl;

    // Calculate constants from Jikken properties
    analysisCount = std::atoi(jikkenGlobals.getProperty("ANALYSISCOUNT").c_str());
    frameBundleSize = std::atoi(jikkenGlobals.getProperty("FRAMEBUNDLESIZE").c_str());
    shapesBlur = std::atoi(jikkenGlobals.getProperty("SHAPESBLUR").c_str()) * 2 - 1;
    shapesBrightnessCutoff = std::atoi(jikkenGlobals.getProperty("SHAPESBRIGHTNESSCUTOFF").c_str());
    brightSpotsBlur = std::atoi(jikkenGlobals.getProperty("BRIGHTSPOTBLUR").c_str()) * 2 - 1;
    brightSpotsAdaptive = jikkenGlobals.getProperty("BRIGHTSPOTADAPTIVE") == "ON";
    brightSpotsAdaptiveC = std::atoi(jikkenGlobals.getProperty("BRIGHTSPOTADAPTIVEC").c_str());
    brightSpotsNonAdaptiveBrightnessCutoff = std::atoi(jikkenGlobals.getProperty("BRIGHTSPOTNONADAPTIVETHRESH").c_str());
    brightSpotsDilationCount = std::atoi(jikkenGlobals.getProperty("BRIGHTSPOTDILATECOUNT").c_str());

    // int analysisCount = 10;

    cv::Mat frame;
    videoIn.read(frame);

    const std::set<std::pair<int, int>> recognizedResolutions = {{512, 480}, {256, 240}, {128, 120}};
    const std::map<std::pair<int, int>, int> frameCountMap = {{{512, 480}, 546}, {{256, 240}, 2184}, {{128, 120}, 8736}};

    int frameCount = videoIn.get(cv::CAP_PROP_FRAME_COUNT);

    if (!frameCount && !recognizedResolutions.count({frame.cols, frame.rows})) {
        jikkenGlobals.log("バブル分析：エラー！！！");
        jikkenGlobals.log("大変！ビデオで０フレームがあります。じっけんプログラムには、壊れないファイルをあげてください。");
        return;
    }
    if (!frameCount){
        jikkenGlobals.log("バブル分析：ビデオでフレーム回数が見当たりません。");
        jikkenGlobals.log("それでも、ビデオを読めます。じっけんプログラムがこの解像度を知りますから。");
        jikkenGlobals.log("ですから、このビデオは多分じっけんプログラムから来ました。");
        frameCount = frameCountMap.at({frame.cols, frame.rows});
    }
    jikkenGlobals.log("バブル分析：スタートです！");

    int interval = frameCount / analysisCount;
    int frameCounter = 1, analyzeCounter = 0;

    jikkenGlobals.log("バブル分析：インターバル　" + std::to_string(interval) + "　フレームである");

    // int frameBundleSize = 15;
    std::vector<cv::Mat> frames;

    for (; !frame.empty() && analyzeCounter < analysisCount; videoIn.read(frame)) {
        if (frames.size() == frameBundleSize) frames.erase(frames.begin());
        cv::Mat greyFrame;
        cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
        frames.push_back(greyFrame.clone());

        if (frameCounter % interval == 0) {
            analyzeCounter++;

            AlgorithmResult moreData;
            auto circles = algorithm(frames, moreData);

            jikkenGlobals.log("分析　" + std::to_string(analyzeCounter) + "番！     " + std::to_string(moreData.bubbleCount) +
                              "つバブルあります。");

            csv << analyzeCounter << "," << moreData.bubbleCount << "," << std::endl;
            for (std::string pictureName : moreData.pictureNames) {
                cv::Mat resized;
                cv::resize(moreData.pictures[pictureName], resized, cv::Size(512, 480));
                cv::imwrite(folderPath + "/" + pictureName + std::to_string(analyzeCounter) + ".png", resized);
            }
        }
        frameCounter++;
    }
}

/**
 * Algorithm top level
 */

cv::Mat getShapes(const std::vector<cv::Mat>& frames);
std::vector<cv::Point> getBrightSpots(const cv::Mat& frame);

struct ComponentInfo {
    int bubbleCount;
    std::vector<cv::Mat> components;
    std::vector<std::vector<cv::Point>> componentSpots;

    // Directly from connectedComponents function
    int numLabels;
    cv::Mat labels, stats, centroids;
};

ComponentInfo getComponentInfo(const cv::Mat& shapes, const std::vector<cv::Point>& brightSpots);
std::vector<cv::Vec3f> getCircles(ComponentInfo& ci);
void makeOverlay(const cv::Mat& frame, cv::Mat& shapes, ComponentInfo& ci, cv::Mat& overlay);

std::vector<cv::Vec3f> algorithm(const std::vector<cv::Mat>& frames, AlgorithmResult& moreData) {
    cv::Mat shapes = getShapes(frames);
    std::vector<cv::Point> brightSpots = getBrightSpots(frames.back());
    ComponentInfo ci = getComponentInfo(shapes, brightSpots);
    std::vector<cv::Vec3f> circles = getCircles(ci);

    cv::Mat overlay;
    makeOverlay(frames.back(), shapes, ci, overlay);

    cv::Mat foundCircles;
    cv::cvtColor(frames.back(), foundCircles, cv::COLOR_GRAY2BGR);
    // std::cout << ", found " << circles.size() << " circles" << std::endl;
    for (cv::Vec3f circle : circles) {
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::circle(foundCircles, center, radius, cv::Scalar(0, 0, 255));
    }

    moreData.pictureNames = {"frame", "overlay", "circle", "shape"};
    moreData.pictures = {{"frame", frames.back()}, {"overlay", overlay}, {"circle", foundCircles}, {"shape", shapes}};
    moreData.bubbleCount = ci.bubbleCount;

    return circles;
}

/**
 * Algorithm subroutines
 */

cv::Mat getShapes(const std::vector<cv::Mat>& frames) {
    const int blurN = shapesBlur, threshN = shapesBrightnessCutoff;

    cv::Mat diff, blur, thresh;

    auto subtractor = cv::createBackgroundSubtractorKNN();
    for (cv::Mat f : frames) subtractor->apply(f, diff);
    cv::GaussianBlur(diff, blur, cv::Size(blurN, blurN), 0);
    cv::threshold(blur, thresh, threshN, 255, cv::THRESH_BINARY);

    // cv::imshow("Diff", upscale(diff));
    // cv::imshow("Shapes", upscale(thresh));
    // cv::waitKey(5);

    return thresh;
}

std::vector<cv::Point> getBrightSpots(const cv::Mat& frame) {
    const bool adaptive = brightSpotsAdaptive;
    int dilateIterations = brightSpotsDilationCount;

    const int aC = brightSpotsAdaptiveC, blurN = brightSpotsBlur, threshN = brightSpotsNonAdaptiveBrightnessCutoff;

    cv::Mat blur, thresh, dilated, precontour;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> spots;

    if (adaptive)
        cv::adaptiveThreshold(frame, thresh, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blurN, aC);
    else {
        cv::GaussianBlur(frame, blur, cv::Size(blurN, blurN), 0);
        cv::threshold(blur, thresh, threshN, 255, cv::THRESH_BINARY);
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

    // cv::imshow("spots precontour", precontour);
    // cv::waitKey(5);

    return spots;
}

ComponentInfo getComponentInfo(const cv::Mat& shapes, const std::vector<cv::Point>& brightSpots) {
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

    // Sort components large to small
    std::vector<int> sortedLabels;
    for (int label = 1; label < ci.numLabels; label++) sortedLabels.push_back(label);
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

// Find largest acceptable radius which does not collide with background
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

    return lower;
}

void addSimpleBubbleCircle(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label) {
    double area = ci.stats.at<int>(label, cv::CC_STAT_AREA);
    double pi = std::acos(-1.0);
    double radius = std::sqrt(area / pi);

    int cx = ci.centroids.at<double>(label, 0);
    int cy = ci.centroids.at<double>(label, 1);

    // For simple shapes, don't check boundary
    cv::Mat background;
    cv::bitwise_not(ci.components[0], background);
    cv::bitwise_xor(background, ci.components[label], background);

    // Squeeze radius to conform to existing circles
    int sqRad = searchRadius(background, cv::Point(cx, cy), radius);
    if (sqRad == 0) return;

    circles.push_back(cv::Vec3f(cx, cy, sqRad));
}

void addComplexBubbleCircles(std::vector<cv::Vec3f>& circles, ComponentInfo& ci, int label) {
    // For complex shapes, check the boundary
    cv::Mat background = ci.components[0];

    int bbw = ci.stats.at<int>(label, cv::CC_STAT_WIDTH);
    int bbh = ci.stats.at<int>(label, cv::CC_STAT_WIDTH);
    int maxRadius = std::min(bbw, bbh) / 2;

    for (cv::Point spot : ci.componentSpots[label]) {
        int radius = searchRadius(background, spot, maxRadius);
        if (radius == 0) continue;
        circles.push_back(cv::Vec3f(spot.x, spot.y, radius));
        cv::circle(background, spot, radius, cv::Scalar(255), -1);
    }
}

void makeOverlay(const cv::Mat& frame, cv::Mat& shapes, ComponentInfo& ci, cv::Mat& overlay) {
    cv::Mat canny;
    cv::cvtColor(frame, overlay, cv::COLOR_GRAY2BGR);

    for (int label = 1; label < ci.numLabels; label++)
        for (auto spot : ci.componentSpots[label]) cv::circle(overlay, spot, 1, cv::Scalar(0, 0, 255));

    const double sigma = 0.33;
    uint8_t mean = cv::mean(shapes)[0];
    double lower = std::max(0.0, (1 - sigma) * mean);
    double upper = std::min(255.0, (1 + sigma) * mean);
    cv::Canny(shapes, canny, lower, upper);
    overlay.setTo(cv::Scalar(255, 0, 0), canny);
}