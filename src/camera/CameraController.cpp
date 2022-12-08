#include "CameraController.h"

#include <chrono>
#include <string>
#include <thread>

#include "CameraButtonProperty.h"
#include "CameraSimpleProperty.h"
#include "CameraToggleProperty.h"

CameraController::CameraController() : mainMenu(CameraMenu(false)) { // Main menu does not start on ESC
    /**
     * SET UP MENU NAVIGATION
    */
    mainMenu.addChild(CameraSimpleProperty("TRIGGERMODE", {"START", "CENTER", "END", "RANDOM"}, false, this));  // ESC on bottom
    mainMenu.addChild(CameraSimpleProperty(
        "FRAMERATE", {"30", "60", "125", "250", "500", "1000", "2000", "3000", "5000", "10000"}, false, this));  // ESC on bottom
    mainMenu.addChild(CameraSimpleProperty(
        "RESOLUTION", {"512 x 480", "512 x 240", "256 x 240", "256 x 120", "128 x 120", "128 x 80", "128 x 34"}, false,
        this));  // ESC on bottom

    CameraMenu idNumberMenu(false);
    idNumberMenu.addChild(CameraButtonProperty("IDNUMBER_CLEAR"));
    idNumberMenu.addChild(CameraButtonProperty("IDNUMBER_INCREMENT"));
    idNumberMenu.addChild(CameraButtonProperty("IDNUMBER_DECREMENT"));
    mainMenu.addChild(idNumberMenu);

    mainMenu.addBlank(); // Gamma tab

    CameraMenu systemMenu(true);
    systemMenu.addBlank(2);  // Edge enhancement and zoom tabs
    CameraMenu displayMenu(false);
    displayMenu.addBlank(); // Date/time toggle
    displayMenu.addChild(CameraToggleProperty("DISPLAY", {"ON1", "ON2", "OFF"}, this));
    systemMenu.addChild(displayMenu);
    systemMenu.addBlank(5); // Date/time set, external IO, Color Temp, Others, Status
    //TOOD: Check number of child in system menu on the hardware
    mainMenu.addChild(systemMenu);

}

bool CameraController::config(std::string serial, std::string id) {
    bool ret = false;
    std::cout << "CameraController config() called, returning " << std::to_string(ret) << std::endl;
    return ret;
}

void CameraController::setCameraProperty(std::string prop, std::string value) {
    if (prop == "FRAMERATE")
        std::cout << "CameraController setting framerate to " << value << std::endl;
    else if (prop == "SHUTTERSPEED")
        std::cout << "CameraController setting shutter speed to " << value << std::endl;
    else if (prop == "TRIGGERMODE")
        std::cout << "CameraController setting trigger mode to " << value << std::endl;
    else
        std::cout << "CameraController was told to set property which does not exist!" << std::endl;
}

std::string CameraController::getCameraProperty(std::string prop) { return ""; }

void CameraController::record() {
    std::cout << "CameraController recording... ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "done!" << std::endl;
}

void CameraController::download() {
    std::cout << "CameraController downloading... ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "done!" << std::endl;
}