#include <vector>

#include "JikkenPropertiesHolder.h"
#include "Logger.h"
#include "StatusSetter.h"
#include "MainManager.h"

class JikkenGlobals {
    MainManager* manager;
    StatusSetter* statusSetter;
    Logger* logger;
    std::unique_ptr<JikkenPropertiesHolder> jikkenPropertiesHolder;
public:
    JikkenGlobals(MainManager* manager, StatusSetter* statusSetter, Logger* logger) {
        this->manager = manager;
        this->statusSetter = statusSetter;
        this->logger = logger;
        jikkenPropertiesHolder = std::unique_ptr<JikkenPropertiesHolder>(new JikkenPropertiesHolder());
    }
    JikkenGlobals() {
        this->statusSetter = nullptr;
        this->logger = nullptr;
    }
    bool update(MainManager::Message msg, bool runHealthCheck = true) { return manager->update(msg, runHealthCheck); }
    void log(std::string text) { logger->log(text); }
    void setStatus(StatusSetter::JikkenState state, bool alsoHasDownload = false) {
        statusSetter->setStatus(state, alsoHasDownload);
    }
    void setProperty(std::string prop, std::string value) { jikkenPropertiesHolder->setProperty(prop, value); }
    const std::string& getProperty(std::string prop) { return jikkenPropertiesHolder->getProperty(prop); }

    // These are useful for the properties panel
    const std::vector<std::string>& propertiesList() { return jikkenPropertiesHolder->getPropertiesList(); }
    const std::string& propertyType(std::string prop) { return jikkenPropertiesHolder->getPropertyType(prop); }
    const std::string& propertyDisplayName(std::string prop) { return jikkenPropertiesHolder->getPropertyDisplayName(prop); }

    // Properties that cannot be set in the properties panel
    // bool inSetup = true;
    // bool cameraOnlyMode = false;
    bool videoPanelSubscribed = false;
};