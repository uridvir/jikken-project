#pragma once

#include <string>
#include <vector>

#include "CameraCommand.h"

class CameraMenuItem {
public:
    virtual bool canSetProperty(std::string prop) = 0;
    virtual std::vector<CameraCommand> setProperty(std::string prop, std::string value) = 0;
    virtual const std::vector<std::string> getOptions(std::string prop) = 0;
};