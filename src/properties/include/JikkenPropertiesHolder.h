#include <iostream>
#include <vector>
#include <map>

class JikkenPropertiesHolder {
    std::vector<std::vector<std::string>> propTable = {
        // Name, display name, type, default value
        {"SERIALPORT", "", "NODISPLAY", "COM3"},
        {"CAMERAID", "", "NODISPLAY", "0"},
        {"AUTODOWNLOAD", "自動ダウンロード", "BOOL", "ON"}
    };
public:
    JikkenPropertiesHolder();
    //Getters and setters
    void setProperty(std::string prop, std::string value);
    const std::string& getProperty(std::string prop);
    const std::vector<std::string>& getPropertiesList();
    const std::string& getPropertyType(std::string prop);
    const std::string& getPropertyDisplayName(std::string prop);
private:
    //File I/O
    bool hasPropertiesFile();
    void fetchProperties();
    void saveProperties();

    std::vector<std::string> propertiesList;
    std::map<std::string, std::string> propertyType;
    std::map<std::string, std::string> propertyDisplayName;
    std::map<std::string, std::string> properties;
};