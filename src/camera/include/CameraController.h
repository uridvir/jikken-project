#pragma once

#include <iostream>

#include "CameraQuerier.h"

class CameraController : public CameraQuerier {
    // CameraSerial serial;
    // CameraVideoStream stream;
    CameraMenu mainMenu;
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