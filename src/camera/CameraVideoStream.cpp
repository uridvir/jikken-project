#include "CameraVideoStream.h"

#include <vector>

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
    if (cap.isOpened()) mutex.lock();
    action();
    if (cap.isOpened()) mutex.unlock();
}

void CameraVideoStream::loop() {
    while (true) {
        const std::lock_guard<std::mutex> lock(mutex);  // Makes the loop thread-safe
        if (!cap.isOpened()) continue;

        for (VideoSubscriber* sub : std::vector<VideoSubscriber*>(subscribers.begin(), subscribers.end()))
            if (sub->isDone()) removeSubscriber(sub);

        cv::Mat frame;
        cap.read(frame);
        for (VideoSubscriber* sub : subscribers) sub->onReceiveMat(frame);
    }
}