#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv[]){
    cv::Mat frame;
    cv::VideoCapture cap;

    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);

    if (!cap.isOpened()){
        std::cerr << "Error! Could not open camera!" << std::endl;
        return -1;
    }

    while (true){
        cap.read(frame);

        if (frame.empty()) {
            std::cerr << "Error! Blank frame grabbed!" << std::endl;
            break;
        }

        cv::imshow("Live", frame);
        if (cv::waitKey(5) >= 0)
            break;
    }

    return 0;
}