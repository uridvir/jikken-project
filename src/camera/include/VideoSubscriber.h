#pragma once

#include <opencv2/core.hpp>

class VideoSubscriber {
public:
    virtual void onReceiveMat(const cv::Mat& mat) = 0;
    virtual bool isDone() { return false; }
    virtual void onRemove() {}
};