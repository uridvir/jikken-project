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

std::string CameraSerial::query(std::string prop, const std::vector<std::string>& options){
    dev.writeChar(queryCode[prop]);
    char c;
    dev.readChar(&c, 100);
    int index = c;
    return options[index];
}

void CameraSerial::execute(CameraCommand cmd){
    dev.writeChar(commandCode[cmd]);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CameraSerial::execute(std::vector<CameraCommand> cmds){
    for (auto cmd : cmds) execute(cmd);
}