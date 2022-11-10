#include "VideoCleaner.h"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <set>
#include <cmath>

cv::Size VideoCleaner::getOutSize(bool removeText){
    cv::Size size;
    size.width = VIDX - XCROP, size.height = VIDY;
    if (removeText)
        size.height -= YCROP_TOP + YCROP_BOTTOM;
    return size;
}

//Return true if mats are different enough
bool matDiffCheck(cv::Mat* mat1, cv::Mat* mat2);

size_t frame2Index(std::vector<cv::Mat>* buffer, cv::Mat* frame1);

//Only some frames should be checked for being the final frame
bool shouldCheckEnd(int frameNum);

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

    int step = 1, frameNum = 1, absFrameNum = 1;
    cv::Mat croppedGrey1;
    
    std::vector<cv::Mat> checkBuffer;
    const size_t bufferMax = 64; // The "step size" for finding frame 2
    checkBuffer.reserve(bufferMax);
    /**
     * Every time the buffer fills up, see if we've passed the frame 1
     * duplicates. In that case, the first non-duplicate (frame 2) is
     * somewhere in the buffer. Search for it.
     * 
     * This way, we spend the bulk of our search time on only a few frames
     * at the end.
    */

    for (; !frame.empty(); videoIn->read(frame)) {
        cv::Mat cropped = frame(cropRegion), croppedGrey = cv::Mat();
        cv::cvtColor(cropped, croppedGrey, cv::COLOR_BGR2GRAY);
        absFrameNum++;

        switch (step) {
            case 1:
                videoOut->write(croppedGrey);
                croppedGrey1 = croppedGrey.clone();  // Clone may be unnecessary
                step = 2;
                break;
            case 2:
                if (checkBuffer.size() < bufferMax)
                    checkBuffer.push_back(croppedGrey.clone());
                if (checkBuffer.size() != bufferMax) break;

                if (matDiffCheck(&croppedGrey, &croppedGrey1)){
                    size_t f2i = frame2Index(&checkBuffer, &croppedGrey1);
                    for (size_t i = f2i; i < bufferMax; i++)
                        videoOut->write(checkBuffer[i]);
                    frameNum += bufferMax - f2i;
                    step = 3;
                    std::cout << "Found frame 2! Absolute position " << absFrameNum - bufferMax + f2i << std::endl;
                }
                checkBuffer.clear();
                break;
            case 3:
                if (shouldCheckEnd(frameNum) && !matDiffCheck(&croppedGrey, &croppedGrey1)){
                    std::cout << "Wrote " << frameNum << " frames." << std::endl;
                    return;
                }
                else {
                    videoOut->write(croppedGrey);
                    frameNum++;
                }
        }
    }
    std::cout << frameNum << std::endl;
}

bool matDiffCheck(cv::Mat* mat1, cv::Mat* mat2){
    cv::Mat resultMatrix;
    cv::matchTemplate(*mat1, *mat2, resultMatrix, cv::TM_SQDIFF_NORMED);

    float resultFloat = resultMatrix.at<float>(0, 0);
    const float threshold = 0.05;
    return resultFloat > threshold;
}

size_t frame2Index(std::vector<cv::Mat>* buffer, cv::Mat* frame1){
    //Perform binary search

    size_t window_start = 0;
    size_t window_size = buffer->size() / 2;

    /**
     * There are two windows, left and right.
     * If frame 2 is in the left window, split the left window
     * into new left and right windows.
     * If not, frame 2 must be in the right window. Move to the
     * right window and do the same thing as with the left.
     * If the window size is zero, we can't split it. We have
     * found frame 2 (at window start).
    */
    while (window_size != 0) {
        size_t index = window_start + window_size - 1;
        if (!matDiffCheck(&buffer->at(index), frame1))
            window_start += window_size;
        window_size = window_size / 2;
    }

    return window_start;
}

bool shouldCheckEnd(int frameNum){
    const std::set<int> validFrameCounts = {546, 2184, 8736}; //From Kodak Motion Corder manual, page 3.5
    const int slop = 2; //Allow for a few skipped frames etc... video encoding is fucky lol

    for (int frameCount : validFrameCounts)
        if (std::abs(frameNum - frameCount) <= slop)
            return true;
    return false;
}