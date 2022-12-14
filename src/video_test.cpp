#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Rect crop(104, 0, 512, 480);
cv::Size size(256, 240);

void greycrop(const cv::Mat& mat, cv::Mat& greycropped){
    cv::Mat cropped = mat(crop), resized = cv::Mat();
    cv::resize(cropped, resized, size);
    cv::cvtColor(resized, greycropped, cv::COLOR_BGR2GRAY);
}

int main(int argc, char* argv[]){
    // cv::Mat frame;
    // cv::VideoCapture cap;

    // int deviceID = 0;
    // int apiID = cv::CAP_ANY;

    // cap.open(deviceID, apiID);

    // if (!cap.isOpened()){
    //     std::cerr << "Error! Could not open camera!" << std::endl;
    //     return -1;
    // }

    // while (true){
    //     cap.read(frame);

    //     if (frame.empty()) {
    //         std::cerr << "Error! Blank frame grabbed!" << std::endl;
    //         break;
    //     }

    //     cv::imshow("Live", frame);
    //     if (cv::waitKey(5) >= 0)
    //         break;
    // }

    cv::Mat frame;
    cv::VideoCapture cap(0, cv::CAP_ANY);
    cap.read(frame);
    cv::resize(frame, frame, cv::Size(720, 480));

    cv::imshow("Frame", frame);
    cv::waitKey(5);

    cv::Mat gc;
    greycrop(frame, gc);

    imshow("gc", gc);
    cv::waitKey(20000);

    return 0;
}