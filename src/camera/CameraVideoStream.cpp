#include "CameraVideoStream.h"
#include "JikkenGlobals.h"

#include <vector>

extern JikkenGlobals jikkenGlobals;

std::mutex mutex;

CameraVideoStream::CameraVideoStream() {
    loopThread = std::thread([this]() { this->loop(); });
    mutex.lock();
    std::cout << "Lock" << std::endl;
}

bool CameraVideoStream::verifyTrulyOpen() {
    if (!cap.isOpened()) return false;
    return cap.grab();
}

bool CameraVideoStream::connect(std::string id) {
    if (connected){
        // cap.release();
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // cap = cv::VideoCapture();
    } 
    threadsafeAction([id, this]() { cap.open(atoi(id.c_str()), cv::CAP_ANY); jikkenGlobals.log("Connecting."); });
    connected = cap.isOpened();
    if (connected)
        jikkenGlobals.log("Video connection.");
    else {
        mutex.lock();
        std::cout << "Lock" << std::endl;
    }
    return connected;
}

void CameraVideoStream::addSubscriber(VideoSubscriber* sub) {
    threadsafeAction([sub, this]() { subscribers.push_back(sub); jikkenGlobals.log("Added subscriber."); });
}

void CameraVideoStream::removeSubscriber(VideoSubscriber* sub) {
    threadsafeAction([sub, this]() {
        subscribers.erase(std::find(subscribers.begin(), subscribers.end(), sub));
        sub->onRemove();
        jikkenGlobals.log("Removed subscriber.");
    });
}

void CameraVideoStream::threadsafeAction(std::function<void()> action) {
    if (cap.isOpened()){
        mutex.lock();
        std::cout << "Lock" << std::endl;
    }
    action();
    if (cap.isOpened()){
        mutex.unlock();
        std::cout << "Unlock" << std::endl;
    }
}

int count = 0; //Debug only

void CameraVideoStream::loop() {
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(mutex);  // Makes the loop thread-safe
            // std::cout << "Loop lock" << std::endl;
            // if (!cap.isOpened()){
            //     std::cout << "Continue" << std::endl;
            //     std::this_thread::sleep_for(std::chrono::milliseconds(50));
            //     continue;
            // }

            for (VideoSubscriber* sub : std::vector<VideoSubscriber*>(subscribers.begin(), subscribers.end()))
                if (sub->isDone()) removeSubscriber(sub);
            // std::cout << "Unlock" << std::endl;
        }

        try {
            if (!cap.isOpened()) continue;

            cv::Mat frame;
            cap.read(frame);
            count = (count + 1) % 100;
            if (count == 0) jikkenGlobals.log("loop alive.");

            const std::lock_guard<std::mutex> lock(mutex);  // Makes the loop thread-safe
            // std::cout << "Lock" << std::endl;
            for (VideoSubscriber* sub : subscribers) sub->onReceiveMat(frame);
            // std::cout << "Unlock" << std::endl;
        }
        catch (std::exception e) {
            std::cout << "Catch" << std::endl;
        }
    }
}