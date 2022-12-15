#include "CameraVideoCleaner.h"

#include <opencv2/imgproc.hpp>
#include <iostream>

#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

void CameraVideoCleaner::onReceiveMat(const cv::Mat& mat) {
    if (!started){
        waitHandle->lock();
        started = true;
    }

    absFrameNum++;

    cv::Mat gcFrame;

    switch (state) {
        case Frame1:
            greycrop(mat, gcFrame1);
            rec->write(gcFrame1);
            state = FindingFrame2;
            break;
        case FindingFrame2:
            if (buff.raws.size() < buffSize) buff.raws.push_back(mat);
            if (buff.raws.size() != buffSize) break;
            gcFrame = buff.gcAt(buffSize - 1);
            if (matDiff(gcFrame, gcFrame1)) {
                size_t f2i = gcFrame2BuffPos();
                for (size_t i = f2i; i < buffSize; i++) rec->write(buff.gcAt(i));
                frameNum += buffSize - f2i;
                state = Main;
                //Download: Frame 2 found! Continuing.
                jikkenGlobals.log("ダウンロード：二番フレームが見つかりました！続けます。");
            } else if (absFrameNum > 200) {
                failure = true;
                done = true;
                //Download: Bad! Frame 2 cannot be found. Stopping.
                jikkenGlobals.log("ダウンロード：大変！二番フレームが見当たりません！やめます。");
                //For bubble analysis, if video is unchanging/static, the Jikken program cannot function.
                jikkenGlobals.log("バブル分析には、全然違わないビデオなら、じっけんプログラムが出来ません。");
            }
            buff.raws.clear();
            buff.gcs.clear();
            break;
        case Main:
            greycrop(mat, gcFrame);
            //TODO: Maybe revamp end detection logic
            if (std::abs(frameNum - expectedFrames) < 5){ //Close enough to end to count as proper finish
                if (!matDiff(gcFrame, gcFrame1)){
                    //Download: Last frame found!
                    jikkenGlobals.log("ダウンロード：最後フレームが見つかりました！");
                    done = true;
                }
            }
            else if (frameNum > expectedFrames){ //Missed the proper end (less than ideal but fine)
                //Download: The last frame cannot be found. This is because the first frame was not seen again.
                jikkenGlobals.log("ダウンロード：最後フレームが見当たりません。一番フレームをもう一回見えなかったですから。");
                //Despite that, all is well! The frame count is only ±5 away from the correct value.
                jikkenGlobals.log("それでも、大丈夫！フレームカウントには、正解カウントから、±５つフレームの中に違いますだけ。");
                done = true;
            }

            if (done) jikkenGlobals.log("ダウンロード：" + std::to_string(frameNum) + "つフレームをファイルに書きました。");
            else {
                rec->write(gcFrame);
                frameNum++;
            }

            break;
    }
}

void CameraVideoCleaner::greycrop(const cv::Mat& mat, cv::Mat& greycropped){
    cv::Mat cropped = mat(crop), resized = cv::Mat();
    cv::resize(cropped, resized, size);
    cv::cvtColor(resized, greycropped, cv::COLOR_BGR2GRAY);
}

//Return true if mats are different enough
bool CameraVideoCleaner::matDiff(const cv::Mat& mat1, const cv::Mat& mat2){
    cv::Mat resultMatrix;
    cv::matchTemplate(mat1, mat2, resultMatrix, cv::TM_SQDIFF_NORMED);

    float resultFloat = resultMatrix.at<float>(0, 0);
    const float threshold = 0.01;
    return resultFloat > threshold;
}

size_t CameraVideoCleaner::gcFrame2BuffPos(){
    //Perform binary search

    size_t window_start = 0;
    size_t window_size = buffSize / 2;

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
        cv::Mat gcFrame = buff.gcAt(index);
        if (!matDiff(gcFrame, gcFrame1))
            window_start += window_size;
        window_size = window_size / 2;
    }
    return window_start;
}