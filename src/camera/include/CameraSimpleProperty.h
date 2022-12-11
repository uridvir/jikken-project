#pragma once

#include "CameraMenuItem.h"

class CameraSimpleProperty : public CameraMenuItem {
    bool escOnTop;

   protected:
    std::string name;
    std::vector<std::string> options;
    CameraController* camCtrl;

   public:
    CameraSimpleProperty(std::string name, const std::vector<std::string>& options, bool escOnTop, CameraController* camCtrl) {
        this->name = name;
        this->options = std::vector<std::string>(options.begin(), options.end());
        this->escOnTop = escOnTop;
        this->camCtrl = camCtrl;
    }
    bool canSetProperty(std::string prop) override final { return prop == name; }
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) override {
        if (!canSetProperty(prop)) return {};

        // Click to enter the property page
        std::vector<CameraCommand> commands = {CameraCommand::MenuEnter};

        // Calculate indices
        std::string current = camCtrl->getCameraProperty(prop);
        int currentIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), current));
        std::string dest = value;
        int destIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), dest));

        // Scroll logic helper
        auto navigate = [&commands, &currentIndex, destIndex]() {
            while (currentIndex != destIndex) {
                commands.push_back(currentIndex < destIndex ? CameraCommand::Down : CameraCommand::Up);
                currentIndex += currentIndex < destIndex ? 1 : -1;
            }
        };

        navigate();                                    // Scroll down (or up) to the requested value
        commands.push_back(CameraCommand::MenuEnter);  // Click
        destIndex = escOnTop ? -1 : options.size();
        navigate();                                    // Scroll to ESC
        commands.push_back(CameraCommand::MenuEnter);  // Click

        return commands;
    }
};