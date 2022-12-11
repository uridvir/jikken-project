#pragma once

#include <memory>

#include "CameraMenuBlank.h"
#include "CameraMenuItem.h"

class CameraMenu : public CameraMenuItem {
    std::vector<std::shared_ptr<CameraMenuItem>> children;
    bool startOnEsc;

   public:
    CameraMenu(bool startOnEsc) { this->startOnEsc = startOnEsc; }
    void addChild(CameraMenuItem* item) { children.push_back(std::shared_ptr<CameraMenuItem>(item)); }
    void addBlank(const int count) {
        for (int i = 0; i < count; i++) addChild(new CameraMenuBlank());
    }
    void addBlank() { addBlank(1); }
    bool canSetProperty(std::string prop) override {
        for (auto& child : children)
            if (child->canSetProperty(prop)) return true;
        return false;
    }
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) override {
        if (!canSetProperty(prop)) return {};

        // Click to enter the menu
        std::vector<CameraCommand> commands = {CameraCommand::MenuEnter};

        // Calculate indices
        int currentIndex = startOnEsc ? children.size() : 0;
        int destIndex = std::distance(children.begin(), std::find_if(children.begin(), children.end(),
                                                                     [prop](auto& item) { return item->canSetProperty(prop); }));

        // Scroll logic helper
        auto navigate = [&commands, &currentIndex, &destIndex]() {
            while (currentIndex != destIndex) {
                commands.push_back(currentIndex < destIndex ? CameraCommand::Down : CameraCommand::Up);
                currentIndex += currentIndex < destIndex ? 1 : -1;
            }
        };

        navigate();                                                          // Scroll to the child that has the property
        auto childCommands = children[destIndex]->setProperty(prop, value);  // Enter child
        commands.insert(commands.end(), childCommands.begin(), childCommands.end());
        destIndex = children.size();
        navigate();                                    // Scroll to ESC
        commands.push_back(CameraCommand::MenuEnter);  // Click

        return commands;
    }
    const std::vector<std::string>& getOptions(std::string prop) override {
        if (!canSetProperty(prop)) return {};
        int destIndex = std::distance(children.begin(), std::find_if(children.begin(), children.end(),
                                                                     [prop](auto& item) { return item->canSetProperty(prop); }));
        return children[destIndex]->getOptions(prop);
    }
};