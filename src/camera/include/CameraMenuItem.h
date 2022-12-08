#include <string>
#include <vector>

#include "CameraCommand.h"

class CameraMenuItem {
    virtual bool canSetProperty(std::string prop) = 0;
    virtual std::vector<CameraCommand> setProperty(std::string prop, std::string value) = 0;
};