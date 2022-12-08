#include "CameraController.h"

#include <string>
#include <thread>
#include <chrono>

CameraController::CameraController(){
    
}

bool CameraController::config(std::string serial, std::string id){
    bool ret = false;
    std::cout << "CameraController config() called, returning " << std::to_string(ret) << std::endl;
    return ret;
}

void CameraController::setCameraProperty(std::string prop, std::string value){
    if (prop == "FRAMERATE")
        std::cout << "CameraController setting framerate to " << value << std::endl;
    else if (prop == "SHUTTERSPEED")
        std::cout << "CameraController setting shutter speed to " << value << std::endl;
    else if (prop == "TRIGGERMODE")
        std::cout << "CameraController setting trigger mode to " << value << std::endl;
    else
        std::cout << "CameraController was told to set property which does not exist!" << std::endl;
}

std::string CameraController::getCameraProperty(std::string prop){
    return "";
}

void CameraController::record(){
    std::cout << "CameraController recording... ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "done!" << std::endl;
}

void CameraController::download(){
    std::cout << "CameraController downloading... ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "done!" << std::endl;
}