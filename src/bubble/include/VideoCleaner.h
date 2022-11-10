#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#define FPS 30

#define VIDX 720
#define VIDY 480

#define XCROP (720 - (256 * 480 / 240)) //Video is 720x480, camera is 256x240
//Ratios for cropping out text were just measured in a photo editor
#define YCROP_TOP (int)(480 * 0.21) //Top text is taller in Display Mode
#define YCROP_BOTTOM (int)(480 * 0.12)

class VideoCleaner {
    public:
    static cv::Size getOutSize(bool removeText);
    static void run(cv::VideoCapture* videoIn, cv::VideoWriter* videoOut, bool removeText);
};