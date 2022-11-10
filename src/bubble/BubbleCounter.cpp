#include "BubbleCounter.h"

#include <opencv2/imgproc.hpp>

void analyze(cv::Mat frame){
    
}

void BubbleCounter::run(cv::VideoCapture* videoIn){
    const int analyzeCount = 10;

    cv::Mat frame;
    int totalFrames = videoIn->get(cv::CAP_PROP_FRAME_COUNT);
    int frameCounter = 0;

    for (videoIn->read(frame); !frame.empty(); videoIn->read(frame)) {
        if (frameCounter % (totalFrames / analyzeCount) == 0)
            analyze(frame);
        frameCounter++;
    }
}
