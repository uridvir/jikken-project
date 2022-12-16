#include "CameraVideoStream.h"

#include <vector>
#include <iostream>

CameraVideoStream::CameraVideoStream() {
    std::thread loopThread([this]() { this->loop(); });
    loopId = loopThread.get_id();
    loopThread.detach(); //It's fine if the loop thread is just suddenly killed, join() is unnecessary
    mutex.lock(); //While the video stream isn't set up, lock the loop
}

bool CameraVideoStream::connect(std::string id) {
    threadsafeAction([id, this]() { cap.open(atoi(id.c_str()), cv::CAP_ANY); });
    return cap.isOpened();
}

void CameraVideoStream::addSubscriber(VideoSubscriber* sub) {
    threadsafeAction([sub, this]() { subscribers.push_back(sub); });
}

void CameraVideoStream::removeSubscriber(VideoSubscriber* sub) {
    threadsafeAction([sub, this]() {
        subscribers.erase(std::find(subscribers.begin(), subscribers.end(), sub));
        sub->onRemove();
    });
}

void CameraVideoStream::threadsafeAction(std::function<void()> action) {
    //Don't touch mutex if called from loop thread
    if (std::this_thread::get_id() == loopId) {
        action();
    }
    else {
        if (cap.isOpened()) mutex.lock(); //If the capture isn't open, the loop is already locked
        action();
        if (cap.isOpened()) mutex.unlock(); //If the capture isn't open, keep the loop locked
    } 
}

void CameraVideoStream::loop() {
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(mutex);  //Thread safety
            for (VideoSubscriber* sub : std::vector<VideoSubscriber*>(subscribers.begin(), subscribers.end()))
                if (sub->isDone()) removeSubscriber(sub);
        }

        if (!cap.isOpened()) continue; //Having this outside the guarded part probably fixes the bug from earlier
        cv::Mat frame;
        cap.read(frame);

        if (frame.empty()) std::cout << "Empty frame!" << std::endl;

        {
            const std::lock_guard<std::mutex> lock(mutex);  // Thread safety
            for (VideoSubscriber* sub : subscribers) sub->onReceiveMat(frame);
        }
    }
}