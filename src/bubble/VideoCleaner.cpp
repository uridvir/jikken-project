#include "VideoCleaner.h"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <set>
#include <cmath>
#include <map>

//Size is for video writer
cv::Size VideoCleaner::getOutSize(bool removeText){
    cv::Size size;
    size.width = VIDX - XCROP, size.height = VIDY;
    if (removeText)
        size.height -= YCROP_TOP + YCROP_BOTTOM;
    return size;
}

cv::Mat greycrop(cv::Mat& frame, cv::Rect& cropRegion){
    cv::Mat cropped = frame(cropRegion), greycropped = cv::Mat();
    cv::cvtColor(cropped, greycropped, cv::COLOR_BGR2GRAY);
    return greycropped;
}

//Return true if mats are different enough
bool matDiffCheck(cv::Mat& mat1, cv::Mat& mat2){
    cv::Mat resultMatrix;
    cv::matchTemplate(mat1, mat2, resultMatrix, cv::TM_SQDIFF_NORMED);

    float resultFloat = resultMatrix.at<float>(0, 0);
    const float threshold = 0.01;
    return resultFloat > threshold;
}

//Only some frames should be checked for being the final frame
bool shouldCheckEnd(int frameNum){
    const std::set<int> validFrameCounts = {546, 2184, 8736}; //From Kodak Motion Corder manual, page 3.5
    const int slop = 5; //Allow for a few skipped frames etc... video encoding is fucky lol

    for (int frameCount : validFrameCounts)
        if (std::abs(frameNum - frameCount) <= slop)
            return true;
    return false;
}

/**
 * Keeps track of uncropped frame buffer, and caches the greycropped frames.
 * So, if a greycropped frame is required, greycropping only needs to happen once.
*/
class GreycropBufferCache {
    public:
    GreycropBufferCache(size_t size, cv::Rect& rect) {
        rawBuffer.reserve(size); 
        cropRegion = rect; 
    }

    //Vector functions
    size_t size(){ return rawBuffer.size(); }
    void push_back(cv::Mat& frame){ rawBuffer.push_back(frame); }
    void clear(){ rawBuffer.clear(); }

    //Caching
    cv::Mat greycropAt(size_t index){
        if (greycrops.count(index) == 1)
            return greycrops.at(index);
        
        cv::Mat greycropped = greycrop(rawBuffer[index], cropRegion);
        greycrops.insert({index, greycropped});
        return greycropped;
    }

    cv::Mat uncroppedAt(size_t index) { return rawBuffer[index]; }

    private:
    std::vector<cv::Mat> rawBuffer;
    std::map<size_t, cv::Mat> greycrops;
    cv::Rect cropRegion;
};

size_t searchBuffer(GreycropBufferCache& cache, cv::Mat& greycropped1);

void VideoCleaner::run(cv::VideoCapture& videoIn, cv::VideoWriter& videoOut, bool removeText) {
    // Video crop changes if there is text
    cv::Rect cropRegion;
    if (removeText)
        cropRegion = cv::Rect(XCROP / 2, YCROP_TOP, VIDX - XCROP, VIDY - YCROP_TOP - YCROP_BOTTOM);
    else
        cropRegion = cv::Rect(XCROP / 2, 0, VIDX - XCROP, VIDY);

    cv::Mat frame, prev;
    videoIn.read(frame);
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
    cv::Mat greycropped, greycropped1;
    
    const size_t bufferSize = 64; // The "step size" for finding frame 2
    GreycropBufferCache bc(bufferSize, cropRegion);
    /**
     * Every time the buffer fills up, see if we've passed the frame 1
     * duplicates. In that case, the first non-duplicate (frame 2) is
     * somewhere in the buffer. Search for it.
     * 
     * This way, we spend the bulk of our search time on only a few frames
     * at the end.
    */

    for (; !frame.empty(); videoIn.read(frame)) {
        absFrameNum++;

        switch (step) {
            case 1:
                greycropped1 = greycrop(frame, cropRegion);
                videoOut.write(greycropped1);
                step = 2;
                break;
            case 2:
                if (bc.size() < bufferSize)
                    bc.push_back(frame.clone());
                if (bc.size() != bufferSize) break;

                greycropped = bc.greycropAt(bufferSize - 1);
                if (matDiffCheck(greycropped, greycropped1)){
                    size_t f2i = searchBuffer(bc, greycropped1);
                    for (size_t i = f2i; i < bufferSize; i++)
                        videoOut.write(bc.greycropAt(i));
                    frameNum += bufferSize - f2i;
                    step = 3;
                    std::cout << "Found frame 2! Absolute position " << absFrameNum - bufferSize + f2i << std::endl;
                    cv::imshow("Frame 2", bc.uncroppedAt(f2i));
                    cv::waitKey(5);
                }
                bc.clear();
                break;
            case 3:
                greycropped = greycrop(frame, cropRegion);
                if (shouldCheckEnd(frameNum) && !matDiffCheck(greycropped, greycropped1)){
                    std::cout << "Wrote " << frameNum << " frames." << std::endl;
                    cv::imshow(std::string("Frame " + std::to_string(frameNum)), prev); 
                    cv::imshow(std::string("Frame " + std::to_string(frameNum + 1)), frame);
                    cv::waitKey(5000);
                    return;
                }
                else {
                    videoOut.write(greycropped);
                    frameNum++;
                }
        }
        prev = frame.clone();
    }
    if (step == 2)
        std::cerr << "Could not locate end of lead-in! No frame 2 found." << std::endl;
    else if (step == 3)
        std::cerr << "Could not locate loop-to-start! No final frame found. Wrote " << frameNum << " frames." << std::endl;
}

size_t searchBuffer(GreycropBufferCache& bc, cv::Mat& greycropped1){
    //Perform binary search

    size_t window_start = 0;
    size_t window_size = bc.size() / 2;

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
        cv::Mat greycropped = bc.greycropAt(index);
        if (!matDiffCheck(greycropped, greycropped1))
            window_start += window_size;
        window_size = window_size / 2;
    }
    return window_start;
}