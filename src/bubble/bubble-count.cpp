#include <iostream>
#include "BubbleCounter.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "bubble-count requires exactly one argument (a video file for counting). Please try again." << std::endl;
        std::cerr << "video-cleanには、正一つコマンドライン引数が必要である"
                  << "（数えるためのビデオファイル）。もう一度試してみてください。" << std::endl;
        return -1;
    }

    std::string videoFilename(argv[1]);
    cv::VideoCapture videoIn(videoFilename);

    BubbleCounter::run(&videoIn);
    videoIn.release();

    return 0;
}