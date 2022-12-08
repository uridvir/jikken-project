#include "CameraSimpleProperty.h"

class CameraToggleProperty : public CameraSimpleProperty {
public:
    CameraToggleProperty(std::string name, const std::vector<std::string>& options, CameraController* camCtrl)
        : CameraSimpleProperty(name, options, false, camCtrl) {}
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) override {
        if (!canSetProperty(prop)) return {};

        std::vector<CameraCommand> commands;

        // Calculate indices
        std::string current = camCtrl->getCameraProperty(prop);
        int currentIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), current));
        std::string dest = value;
        int destIndex = std::distance(options.begin(), std::find(options.begin(), options.end(), dest));

        //We cannot go backwards, so must wrap
        int dist = (destIndex - currentIndex) % options.size();
        for (int i = 0; i < dist; i++) commands.push_back(MenuEnter);

        return commands;
    }
};