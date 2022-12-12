#include "JikkenPropertiesHolder.h"

#include <fstream>
#include <string>

const std::string filename = "properties.dat";

JikkenPropertiesHolder::JikkenPropertiesHolder(){
    std::map<std::string, std::string> defaultValueMap;
    for (auto line : propTable) {
        std::string prop = line[0], displayName = line[1], type = line[2], defaultValue = line[3];
        propertiesList.push_back(prop);
        propertyType.insert({prop, type});
        propertyDisplayName.insert({prop, displayName});
        defaultValueMap.insert({prop, defaultValue});
    }

    if (hasPropertiesFile()) { 
        fetchProperties();
    }
    else {
        for (std::string prop : propertiesList)
            properties.insert({prop, defaultValueMap[prop]});
        saveProperties();
    }
}

void JikkenPropertiesHolder::setProperty(std::string prop, std::string value) {
    properties[prop] = value;
    saveProperties();
}

const std::string& JikkenPropertiesHolder::getProperty(std::string prop) {
    return properties[prop];
}

const std::vector<std::string>& JikkenPropertiesHolder::getPropertiesList() {
    return propertiesList;
}

const std::string& JikkenPropertiesHolder::getPropertyType(std::string prop) {
    return propertyType[prop];
}

const std::string& JikkenPropertiesHolder::getPropertyDisplayName(std::string prop) {
    return propertyDisplayName[prop];
}

/**
 * File I/O
*/
bool JikkenPropertiesHolder::hasPropertiesFile() {
    std::ifstream file(filename);
    return file.is_open();
}

void JikkenPropertiesHolder::fetchProperties() {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)){
        int colonIndex = line.find_first_of(":");
        std::string prop = line.substr(0, colonIndex);
        std::string value = line.substr(colonIndex + 1);
        properties.insert({prop, value});
    }
}

void JikkenPropertiesHolder::saveProperties() {
    std::ofstream file(filename);
    for (std::string prop : propertiesList)
        file << prop << ":" << properties[prop] << std::endl;
}