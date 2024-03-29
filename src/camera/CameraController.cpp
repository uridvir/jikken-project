#include "CameraController.h"

#include <chrono>
#include <string>
#include <thread>

#include "CameraMenu.h"
#include "CameraMenuWrapperProperty.h"
#include "CameraSimpleProperty.h"
#include "CameraToggleProperty.h"
#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

CameraController::CameraController() {
    /**
     * SET UP MENU NAVIGATION
     */
    CameraMenu* mainMenu = new CameraMenu(false);  // Main menu does not start on ESC
    mainMenu->addChild(
        new CameraSimpleProperty("TRIGGERMODE", {"START", "CENTER", "END", "RANDOM"}, false, this));  // ESC on bottom
    mainMenu->addChild(new CameraSimpleProperty(
        "FRAMERATE", {"30", "60", "125", "250", "500", "1000", "2000", "3000", "5000", "10000"}, true, this));  // ESC on top
    mainMenu->addChild(new CameraSimpleProperty(
        "RESOLUTION", {"512 x 480", "512 x 240", "256 x 240", "256 x 120", "128 x 120", "128 x 80", "128 x 34"}, false,
        this));  // ESC on bottom

    mainMenu->addBlank(2);  // ID and Gamma tabs

    CameraMenu* systemMenu = new CameraMenu(true);
    systemMenu->addBlank(2);  // Edge enhancement and zoom tabs
    CameraMenu* displayMenu = new CameraMenu(true);
    displayMenu->addBlank();  // Date/time toggle
    displayMenu->addChild(new CameraToggleProperty("DISPLAY", {"OFF", "ON1", "ON2"}, this));
    systemMenu->addChild(displayMenu);
    systemMenu->addBlank(5);  // Date/time set, external IO, Color Temp, Others, Status
    // TOOD: Check number of child in system menu on the hardware
    mainMenu->addChild(systemMenu);

    CameraMenuWrapperProperty* shutterspeedMenu = new CameraMenuWrapperProperty(
        "SHUTTERSPEED", {"1/30", "1/60", "1/125", "1/250", "1/500", "1/1000", "1/2000", "1/3000", "1/5000", "1/10000", "1/20000"},
        this);
    shutterspeedMenu->addChild(mainMenu);
    menuRoot = std::shared_ptr<CameraMenuItem>(shutterspeedMenu);
}

bool CameraController::healthCheck() {
    // This is assuming that serial is set up, though might not be connected to camera

    // setCameraProperty("IDNUMBER_CLEAR", "");
    int idnumber = serial.query("IDNUMBER");
    if (idnumber < 1 || 99 < idnumber) return false;

    return true;
}

bool CameraController::config() {
    // config has already been called from GUI side

    if (video && jikkenGlobals.videoPanelSubscribed) {
        jikkenGlobals.videoPanelSubscribed = false;
        stream.removeSubscriber(video);
    }

    bool serialSuccess = serial.connect(jikkenGlobals.getProperty("SERIALPORT"));
    bool streamSuccess = stream.connect(jikkenGlobals.getProperty("CAMERAID"));

    if (!serialSuccess || !streamSuccess) return false;

    if (!healthCheck()) return false;

    stream.addSubscriber(video);
    jikkenGlobals.videoPanelSubscribed = true;

    //Make sure display text is on
    if (serial.query("MODE") == 0)
        serial.execute(CameraCommand::Mode);
    setCameraProperty("DISPLAY", "ON1");

    return true;
}

void CameraController::setCameraProperty(std::string prop, std::string value) {
    if (getCameraProperty(prop) == value) return;

    auto commands = menuRoot->setProperty(prop, value);

    if (prop == "FRAMERATE" && value == "3000") { //Overheat protection message interrupts normal navigation
        auto it = commands.begin();
        for (int i = 0; i < 3; i++) it = ++std::find(it, commands.end(), CameraCommand::MenuEnter);
        auto menuClick3 = it - 1; // Main menu (#1), framerate (#2), select 3000fps (#3)
        serial.execute(std::vector<CameraCommand>(commands.begin(), menuClick3 + 1));
        std::this_thread::sleep_for(std::chrono::seconds(6)); //Wait for message to go away
        serial.execute(std::vector<CameraCommand>(menuClick3 + 1, commands.end()));
    }

    else serial.execute(commands);

    std::this_thread::sleep_for(std::chrono::seconds(1)); //PLEASE WAIT... screen

    if (prop == "FRAMERATE") {
        const std::map<std::string, std::string> correctResolution = {
            {"1000", "256 x 240"},
            {"2000", "128 x 120"},
            {"3000", "128 x 120"}
        };
        std::string framerate = value;
        setCameraProperty("RESOLUTION", correctResolution.at(framerate));
    }
}

std::string CameraController::getCameraProperty(std::string prop) { return serial.query(prop, menuRoot->getOptions(prop)); }

const std::map<std::string, int> frameCount = {{"512 x 480", 546}, {"256 x 240", 2184}, {"128 x 120", 8736}};

void CameraController::record() {
    std::string resolution = getCameraProperty("RESOLUTION");
    std::string framerate = getCameraProperty("FRAMERATE");

    double frames = frameCount.at(resolution);
    double fps = std::atoi(framerate.c_str());
    int recordTimeMillis = frames / fps * 1000;

    if (serial.query("RECREADY") == 0) serial.execute(CameraCommand::RecReady);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    serial.execute(CameraCommand::Trigger);
    std::this_thread::sleep_for(std::chrono::milliseconds(recordTimeMillis + 50));
}

void CameraController::download(bool& cleanerFailure) {
    // Setup for cleaner
    std::mutex* waitHandle = new std::mutex();
    int skew = std::atoi(jikkenGlobals.getProperty("VIDEOSKEW").c_str());
    cv::Rect crop((720 - 512) / 2 + skew, 0, 512, 480);

    std::string resolution = getCameraProperty("RESOLUTION");
    const std::map<std::string, cv::Size> sizeMap = {
        {"512 x 480", cv::Size(512, 480)}, {"256 x 240", cv::Size(256, 240)}, {"128 x 120", cv::Size(128, 120)}};
    cv::Size size = sizeMap.at(resolution);

    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    cv::VideoWriter* rec = new cv::VideoWriter("vid.avi", fourcc, 30, size, false);
    int frames = frameCount.at(resolution);

    // Get ready to record
    setCameraProperty("DISPLAY", "OFF");
    serial.execute(CameraCommand::Mode);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait for display mode to be ready

    CameraVideoCleaner* cleaner = new CameraVideoCleaner(rec, crop, size, frames, waitHandle);
    stream.addSubscriber(cleaner);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Give cleaner time to start
    serial.execute(CameraCommand::Play);
    waitHandle->lock();  // Wait for cleaner
    waitHandle->unlock();
    serial.execute(CameraCommand::Mode);
    setCameraProperty("DISPLAY", "ON1");

    cleanerFailure = cleaner->failure;

    delete waitHandle, rec, cleaner;
}

void CameraController::assignMonitor(VideoSubscriber* video) { this->video = video; }