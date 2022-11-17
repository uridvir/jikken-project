#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class BubbleCounter {
    public:
    static void run(cv::VideoCapture& videoIn);
};