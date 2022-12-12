#include <vector>

#include "JikkenPropertiesHolder.h"
#include "Logger.h"
#include "StatusSetter.h"

class JikkenGlobals {
    StatusSetter* statusSetter;
    Logger* logger;
    std::unique_ptr<JikkenPropertiesHolder> jikkenPropertiesHolder;
public:
    JikkenGlobals(StatusSetter* statusSetter, Logger* logger) {
        this->statusSetter = statusSetter;
        this->logger = logger;
        jikkenPropertiesHolder = std::unique_ptr<JikkenPropertiesHolder>(new JikkenPropertiesHolder());
    }
    JikkenGlobals() {
        this->statusSetter = nullptr;
        this->logger = nullptr;
    }
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
    bool cameraOnlyMode = false;
    bool videoPanelSubscribed = false;
};