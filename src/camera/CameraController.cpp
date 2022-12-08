#include "CameraController.h"

#include <string>

CameraController::CameraController(){
    
}

bool CameraController::config(std::string serial, int id){
    bool ret = false;
    std::cout << "CameraController config() called, returning " << std::to_string(ret) << std::endl;
    return ret;
}

void CameraController::setCameraProperty(std::string prop, std::string value){

}

void CameraController::record(){

}

void CameraController::download(){

}