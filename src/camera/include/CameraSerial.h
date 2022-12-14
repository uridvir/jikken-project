#include "serialib.h"
#include "CameraCommand.h"

#include <iostream>
#include <vector>
#include <map>

class CameraSerial {
    serialib dev;
    bool connected;
public:
    bool connect(std::string port);
    int query(std::string prop);
    std::string query(std::string prop, const std::vector<std::string>& options);
    void execute(CameraCommand cmd);
    void execute(std::vector<CameraCommand> cmds);
private:
    std::map<std::string, char> queryCode = {
        {"TRIGGERMODE", 0x87},
        {"FRAMERATE", 0x89},
        {"RESOLUTION", 0x8A},
        {"SHUTTERSPEED", 0xA6},
        {"DISPLAY", 0xBA},
        {"MODE", 0x83}
    };
    std::map<CameraCommand, char> commandCode = {
        {CameraCommand::Play, 0x65},
        {CameraCommand::Esc, 0x68},
        {CameraCommand::Down, 0x69},
        {CameraCommand::Up, 0x6A},
        {CameraCommand::Mode, 0x71},
        {CameraCommand::MenuEnter, 0x72},
        {CameraCommand::RecReady, 0x73},
        {CameraCommand::Trigger, 0x74}
    };
};