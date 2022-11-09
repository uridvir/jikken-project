#include <iostream>
#include "VideoCleaner.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>

std::string getVideoOutFilename(std::string videoFilename);
bool checkTextRemove(std::string videoFilename);

int main(int argc, char* argv[]){
    /**
     * Note: To ask the cleaner to remove display text, put "txt" somewhere in the filename.
     * メモ：video-cleanに写真のディスプレイテクストを除きたい場合は、ファイル名の中に"txt"を入れてください。
    */

    if (argc != 2) {
        std::cerr << "video-clean requires exactly one argument (a video file to clean). Please try again." << std::endl;
        std::cerr << "video-cleanには、正一つコマンドライン引数が必要である"
                  << "（掃除するためのビデオファイル）。もう一度試してみてください。" << std::endl;
        return -1;
    }

    std::string videoFilename(argv[1]);
    cv::VideoCapture videoIn(videoFilename);
    VideoCleaner::run(&videoIn, getVideoOutFilename(videoFilename), checkTextRemove(videoFilename));

    return 0;
}

std::string getVideoOutFilename(std::string videoFilename){
    size_t lastDot = videoFilename.find_last_of(".");
    return "out-" + videoFilename.substr(0, lastDot) + ".avi";
}

bool checkTextRemove(std::string videoFilename){
    return videoFilename.find("txt") != std::string::npos;
}