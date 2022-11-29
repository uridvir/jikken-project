// Main program
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "JikkenFrame.h"
#include "ConfigPanel.h"
#include "VideoPanel.h"
#include "SettingsPanel.h"
#include "DownloadPanel.h"
#include "StatusPanel.h"
 
class JikkenApp : public wxApp
{
    JikkenFrame* frame;
    ConfigPanel* configPanel;
    VideoPanel* videoPanel;
    SettingsPanel* settingsPanel;
    DownloadPanel* downloadPanel;
    StatusPanel* statusPanel;

public:
    bool OnInit(){
        //Make elements
        frame = new JikkenFrame();
        configPanel = new ConfigPanel(frame);
        videoPanel = new VideoPanel(frame);
        settingsPanel = new SettingsPanel(frame);
        downloadPanel = new DownloadPanel(frame);
        statusPanel = new StatusPanel(frame);

        //Sizers
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(configPanel, 0, wxEXPAND);
        sizer->Add(videoPanel, 1, wxEXPAND);
        //sizer->Add(settingsPanel);

        //Set up frame
        frame->SetSizer(sizer);
        frame->SetMinClientSize(wxSize(250, 150));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(JikkenApp);