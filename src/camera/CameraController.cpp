#include "CameraController.h"

#include <string>

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

}

void CameraController::download(){

}