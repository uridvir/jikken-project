#pragma once

#include <iostream>
#include <map>

#include "CameraQuerier.h"
#include "CameraSerial.h"
#include "CameraVideoStream.h"
#include "CameraMenuItem.h"

class CameraController : public CameraQuerier {
    std::map<std::string, std::string> properties; //Only for debug

    CameraSerial serial;
    CameraVideoStream stream;
    std::shared_ptr<CameraMenuItem> menuRoot;
    VideoSubscriber* video;
    // cv::VideoWriter rec;
    // VideoCleaner cleaner;
public:
    CameraController();
    bool config();
    void setCameraProperty(std::string prop, std::string value);
    std::string getCameraProperty(std::string prop) override; //Differs from class diagram
    void record();
    void download();
    void assignMonitor(VideoSubscriber* video);
};