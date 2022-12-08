#pragma once

#include <iostream>

class CameraController {
    // CameraSerial serial;
    // CameraVideoStream stream;
    // CameraMenu mainMenu;
    // cv::VideoWriter rec;
    // VideoCleaner cleaner;
public:
    CameraController();
    bool config(std::string port, int id);
    void setCameraProperty(std::string prop, std::string value);
    void record();
    void download();
};