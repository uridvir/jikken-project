#include "VideoCleaner.h"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Size VideoCleaner::getOutSize(bool removeText){
    cv::Size size;
    size.width = VIDX - XCROP, size.height = VIDY;
    if (removeText)
        size.height -= YCROP_TOP + YCROP_BOTTOM;
    return size;
}

//Return true if mats are different enough
bool matDiffCheck(cv::Mat* mat1, cv::Mat* mat2);

void VideoCleaner::run(cv::VideoCapture* videoIn, cv::VideoWriter* videoOut, bool removeText) {
    // Video crop changes if there is text
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

    /**
     * Steps:
     * 1) Write first frame
     * 2) Skip copies of first frame
     * 3) Write frames, terminate on copy of first frame
     * 
     * The trimming will not work if the video has text and is not labeled as such.
     */

    int step = 1, frameNum = 1;
    cv::Mat croppedGrey1;

    for (; !frame.empty(); videoIn->read(frame)) {
        cv::Mat cropped = frame(cropRegion), croppedGrey = cv::Mat();
        cv::cvtColor(cropped, croppedGrey, cv::COLOR_BGR2GRAY);

        switch (step) {
            case 1:
                videoOut->write(croppedGrey);
                croppedGrey1 = croppedGrey.clone();  // Clone may be unnecessary
                step = 2;
                break;
            case 2:
                if (matDiffCheck(&croppedGrey, &croppedGrey1)){
                    step = 3;
                    std::cout << "Found frame 2!" << std::endl;
                }
                else break;
            case 3:
                if (!matDiffCheck(&croppedGrey, &croppedGrey1)){
                    std::cout << "Wrote " << frameNum << " frames." << std::endl;
                    return;
                }
                else {
                    videoOut->write(croppedGrey);
                    frameNum++;
                }
        }
    }
}

bool matDiffCheck(cv::Mat* mat1, cv::Mat* mat2){
    cv::Mat resultMatrix;
    cv::matchTemplate(*mat1, *mat2, resultMatrix, cv::TM_SQDIFF_NORMED);

    float resultFloat = resultMatrix.at<float>(0, 0);
    const float threshold = 0.05;
    return resultFloat > threshold;
}