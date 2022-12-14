#include "CameraVideoStream.h"

#include <vector>
#include <iostream>

std::mutex mutex;

CameraVideoStream::CameraVideoStream() {
    loopThread = std::thread([this]() { this->loop(); });
    mutex.lock();
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
    if (std::this_thread::get_id() == loopThread.get_id()) {
        action();
    }
    else {
        if (cap.isOpened()) mutex.lock();
        action();
        if (cap.isOpened()) mutex.unlock();
    } 
}

//TODO: Proper exit to avoid terminate exception
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