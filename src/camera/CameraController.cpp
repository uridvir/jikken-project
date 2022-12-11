#include "CameraController.h"

#include <chrono>
#include <string>
#include <thread>

#include "CameraButtonProperty.h"
#include "CameraMenu.h"
#include "CameraMenuWrapperProperty.h"
#include "CameraSimpleProperty.h"
#include "CameraToggleProperty.h"

CameraController::CameraController() {
    /**
     * SET UP MENU NAVIGATION
     */
    CameraMenu* mainMenu = new CameraMenu(false);  // Main menu does not start on ESC
    mainMenu->addChild(
        new CameraSimpleProperty("TRIGGERMODE", {"START", "CENTER", "END", "RANDOM"}, false, this));  // ESC on bottom
    mainMenu->addChild(new CameraSimpleProperty(
        "FRAMERATE", {"30", "60", "125", "250", "500", "1000", "2000", "3000", "5000", "10000"}, false, this));  // ESC on bottom
    mainMenu->addChild(new CameraSimpleProperty(
        "RESOLUTION", {"512 x 480", "512 x 240", "256 x 240", "256 x 120", "128 x 120", "128 x 80", "128 x 34"}, false,
        this));  // ESC on bottom

    CameraMenu* idNumberMenu = new CameraMenu(false);
    idNumberMenu->addChild(new CameraButtonProperty("IDNUMBER_CLEAR"));
    idNumberMenu->addChild(new CameraButtonProperty("IDNUMBER_INCREMENT"));
    idNumberMenu->addChild(new CameraButtonProperty("IDNUMBER_DECREMENT"));
    mainMenu->addChild(idNumberMenu);

    mainMenu->addBlank();  // Gamma tab

    CameraMenu* systemMenu = new CameraMenu(true);
    systemMenu->addBlank(2);  // Edge enhancement and zoom tabs
    CameraMenu* displayMenu = new CameraMenu(false);
    displayMenu->addBlank();  // Date/time toggle
    displayMenu->addChild(new CameraToggleProperty("DISPLAY", {"ON1", "ON2", "OFF"}, this));
    systemMenu->addChild(displayMenu);
    systemMenu->addBlank(5);  // Date/time set, external IO, Color Temp, Others, Status
    // TOOD: Check number of child in system menu on the hardware
    mainMenu->addChild(systemMenu);

    CameraMenuWrapperProperty* shutterspeedMenu = new CameraMenuWrapperProperty(
        "SHUTTERSPEED", {"1/30", "1/60", "1/125", "1/250", "1/500", "1/1000", "1/2000", "1/3000", "1/5000", "1/10000", "1/20000"},
        this);
    shutterspeedMenu->addChild(mainMenu);
    menuRoot = std::shared_ptr<CameraMenuItem>(shutterspeedMenu);

    // Debug
    properties.insert({"FRAMERATE", "1000"});
    properties.insert({"RESOLUTION", "256 x 240"});
    properties.insert({"SHUTTERSPEED", "1/1000"});
    properties.insert({"TRIGGERMODE", "START"});
    properties.insert({"DISPLAY", "ON1"});
}

bool CameraController::config(std::string serial, std::string id) {
    bool ret = false;
    std::cout << "CameraController config() called, returning " << std::to_string(ret) << std::endl;
    return ret;
}

void CameraController::setCameraProperty(std::string prop, std::string value) {
    auto commands = menuRoot->setProperty(prop, value);
    std::cout << "CameraController sent commands ";
    auto commandToString = [](CameraCommand command) {
        switch (command) {
            case CameraCommand::RecReady:
                return "RecReady";
            case CameraCommand::Trigger:
                return "Trigger";
            case CameraCommand::Mode:
                return "Mode";
            case CameraCommand::Play:
                return "Play";
            case CameraCommand::Esc:
                return "Esc";
            case CameraCommand::Down:
                return "Down";
            case CameraCommand::MenuEnter:
                return "MenuEnter";
            case CameraCommand::Up:
                return "Up";
        }
        return "";
    };
    for (auto command : commands) std::cout << commandToString(command) << " ";
    std::cout << std::endl;
    properties[prop] = value;
}

std::string CameraController::getCameraProperty(std::string prop) { return properties.at(prop); }

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