#include <iostream>
#include "VideoCleaner.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

void VideoCleaner::run(cv::VideoCapture* videoIn, cv::VideoWriter* videoOut, bool removeText){
    cv::Rect cropRegion;
    if (removeText)
        cropRegion = cv::Rect(XCROP / 2, YCROP_TOP, VIDX - XCROP, VIDY - YCROP_TOP - YCROP_BOTTOM);
    else
        cropRegion = cv::Rect(XCROP / 2, 0, VIDX - XCROP, VIDY);

    cv::Mat frame;
    videoIn->read(frame);
    if (frame.cols != VIDX || frame.rows != VIDY) {
        std::cerr << "Video size is wrong! Must be " << VIDX << "x" << VIDY << "." << std::endl;
        std::cerr << "ビデオサイズが違います！正解のサイズが" << VIDX << "x" << VIDY << "である。" << std::endl;
        return;
    }

    cv::Mat _frame = frame.clone();
    cv::rectangle(_frame, cropRegion, cv::Scalar(0, 0, 255));
    cv::imshow("Crop region", _frame);
    cv::waitKey(5);

    for (; !frame.empty(); videoIn->read(frame)){
        cv::Mat cropped = frame(cropRegion), croppedGrey = cv::Mat();
        cv::cvtColor(cropped, croppedGrey, cv::COLOR_BGR2GRAY);
        videoOut->write(croppedGrey);
    }
}