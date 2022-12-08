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

}

std::string CameraController::getCameraProperty(std::string prop){
    return "";
}

void CameraController::record(){

}

void CameraController::download(){

}