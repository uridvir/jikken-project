#pragma once

#include "CameraMenuItem.h"

class CameraMenuBlank : public CameraMenuItem {
public:
    bool canSetProperty(std::string prop) { return false; }
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) { return std::vector<CameraCommand>(); }
    const std::vector<std::string>& getOptions(std::string prop) { return {}; }
};