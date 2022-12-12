#include "VideoSubscriber.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class CameraVideoStream {
    cv::VideoCapture cap;
    std::vector<VideoSubscriber*> subscribers;
    std::thread loopThread;
public:
    CameraVideoStream();
    bool connect(std::string id);
    void addSubscriber(VideoSubscriber* sub);
    void removeSubscriber(VideoSubscriber* sub);
private:
    void loop();
    void threadsafeAction(std::function<void()> action);
};