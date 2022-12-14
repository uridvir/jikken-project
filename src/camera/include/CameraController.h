#pragma once

#include <iostream>
#include <map>

#include "CameraQuerier.h"
#include "CameraSerial.h"
#include "CameraVideoStream.h"
#include "CameraMenuItem.h"
#include "CameraVideoCleaner.h"

class CameraController : public CameraQuerier {
    CameraSerial serial;
    CameraVideoStream stream;
    std::shared_ptr<CameraMenuItem> menuRoot;
    VideoSubscriber* video;
public:
    CameraController();
    bool healthCheck();
    bool config();
    void setCameraProperty(std::string prop, std::string value);
    std::string getCameraProperty(std::string prop) override; //Differs from class diagram
    void record();
    void download();
    void assignMonitor(VideoSubscriber* video);
};