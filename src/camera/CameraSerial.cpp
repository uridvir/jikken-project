#include "CameraSerial.h"

#include <chrono>
#include <thread>

bool CameraSerial::connect(std::string port) {
    if (connected){
        dev.closeDevice();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        dev = serialib();
    } 
    dev.openDevice(port.c_str(), 4800, SERIAL_DATABITS_8, SERIAL_PARITY_NONE, SERIAL_STOPBITS_2);
    connected = dev.isDeviceOpen();
    return connected;
}

int CameraSerial::query(std::string prop){
    dev.writeChar(queryCode[prop]);
    char c = 0;
    dev.readChar(&c, 100);
    return c;
}

std::string CameraSerial::query(std::string prop, const std::vector<std::string>& options){
    int index = query(prop);

    if (prop == "SHUTTERSPEED") //Shutterspeed querying is relative for some reason, wtf???
        index += query("FRAMERATE"); //Original index was relative to current FPS
    
    if (prop == "RESOLUTION") //Resolution options are backwards, again wtf???
        return options[(options.size() - 1) - index];
    
    return options[index];
}

void CameraSerial::execute(CameraCommand cmd){
    dev.writeChar(commandCode[cmd]);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CameraSerial::execute(std::vector<CameraCommand> cmds){
    for (auto cmd : cmds) execute(cmd);
}