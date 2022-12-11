#pragma once

#include "CameraSimpleProperty.h"
#include "CameraMenu.h"

class CameraMenuWrapperProperty : public CameraSimpleProperty {
    CameraMenu child;
public:
    CameraMenuWrapperProperty(std::string name, const std::vector<std::string>& options, CameraQuerier* camQuer)
        : CameraSimpleProperty(name, options, false, camQuer) {}
    CameraMenuWrapperProperty() : CameraMenuWrapperProperty("", {}, nullptr) {}
    void addChild(CameraMenu child){
        this->child = child;
    }
    bool canSetProperty(std::string prop) override {
        if (prop == name) return true;
        return child.canSetProperty(prop);
    }
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) override {
        if (!canSetProperty(prop)) return {};

        if (prop != name) return child.setProperty(prop, value); // Outsource to child

        std::vector<CameraCommand> commands;

        // Calculate indices
        std::string current = camQuer->getCameraProperty(prop);
        int currentIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), current));
        std::string dest = value;
        int destIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), dest));

        // Scroll
        while (currentIndex != destIndex) {
            // Options in ternary are swapped (Up sets a higher shutter speed)
            commands.push_back(currentIndex < destIndex ? CameraCommand::Up : CameraCommand::Down);
            currentIndex += currentIndex < destIndex ? 1 : -1;
        }

        return commands;
    }
};