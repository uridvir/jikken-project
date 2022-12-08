#include "CameraToggleProperty.h"

class CameraButtonProperty : public CameraToggleProperty {
   public:
    CameraButtonProperty(std::string name) : CameraToggleProperty(name, {}, nullptr) {}
    std::vector<CameraCommand> setProperty(std::string prop, std::string value) override {
        return canSetProperty(prop) ? std::vector<CameraCommand>({CameraCommand::MenuEnter}) : std::vector<CameraCommand>({});
    }
};