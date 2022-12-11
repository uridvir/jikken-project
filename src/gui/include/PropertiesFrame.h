// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <map>

class PropertiesFrame : public wxFrame {
    const std::vector<char*[2]> propList = {
        {"AUTODOWNLOAD", "BOOL"}
    };
    const std::vector<std::string> textPropList = {};
    std::map<std::string, std::string> propDisplayName = {
        {"AUTODOWNLOAD", "自動ダウンロード"}
    };
public:
    PropertiesFrame();
};