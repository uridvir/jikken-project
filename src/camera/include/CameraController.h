#pragma once

#include <iostream>
#include <map>

#include "CameraQuerier.h"
#include "CameraMenuItem.h"

class CameraController : public CameraQuerier {
    std::map<std::string, std::string> properties; //Only for debug

    // CameraSerial serial;
    // CameraVideoStream stream;
    std::shared_ptr<CameraMenuItem> menuRoot;
    // cv::VideoWriter rec;
    // VideoCleaner cleaner;
public:
    CameraController();
    bool config(std::string port, std::string id);
    void setCameraProperty(std::string prop, std::string value);
    std::string getCameraProperty(std::string prop) override; //Differs from class diagram
    void record();
    void download();
};