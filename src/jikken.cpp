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
 
class JikkenApp : public wxApp
{
    JikkenFrame* frame;
    ConfigPanel* configPanel;
    VideoPanel* videoPanel;
    SettingsPanel* settingsPanel;

public:
    bool OnInit(){
        //Make elements
        frame = new JikkenFrame();
        configPanel = new ConfigPanel(frame);
        videoPanel = new VideoPanel(frame);
        settingsPanel = new SettingsPanel(frame);

        //Sizers
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(configPanel, 1, wxEXPAND);
        //sizer->Add(videoPanel);
        //sizer->Add(settingsPanel);

        //Set up frame
        frame->SetSizer(sizer);
        frame->SetMinClientSize(wxSize(250, 150));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(JikkenApp);