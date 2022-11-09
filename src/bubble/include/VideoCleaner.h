#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#define FPS 30

class VideoCleaner {
    public:
    static void run(cv::VideoCapture* videoIn, std::string videoOutFilename, bool removeText);
};