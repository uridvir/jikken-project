#include <iostream>
#include <vector>
#include <map>

class JikkenPropertiesHolder {
    std::vector<std::vector<std::string>> propTable = {
        // Name, display name, type, default value
        {"SERIALPORT", "", "NODISPLAY", "COM3"},
        {"CAMERAID", "", "NODISPLAY", "0"},
        {"AUTODOWNLOAD", "自動ダウンロード", "BOOL", "入"},
        {"VIDEOSKEW", "ビデオ’Ｘ’オフセット", "INT-50:50", "-40"},
        {"ANALYSISCOUNT", "分析回数", "INT1:150", "10"},
        {"FRAMEBUNDLESIZE", "フレーム集サイズ", "INT1:100", "15"},
        {"SHAPESBLUR", "形のぼかし", "INT1:10", "3"},
        {"SHAPESBRIGHTNESSCUTOFF", "形のしきい値処理の番号", "INT0:100", "10"},
        {"BRIGHTSPOTBLUR", "起点のぼかし", "INT1:10", "3"},
        {"BRIGHTSPOTADAPTIVE", "起点のアダプティブモード", "BOOL", "入"},
        {"BRIGHTSPOTADAPTIVEC", "起点のアダプティブモードの一定", "INT-50:50", "-10"},
        {"BRIGHTSPOTNONADAPTIVETHRESH", "起点のノンアダプティブモードのしきい値処理の番号", "INT0:150", "100"},
        {"BRIGHTSPOTDILATECOUNT", "起点の拡張回数", "INT0:10", "2"}
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