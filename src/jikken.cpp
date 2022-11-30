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
        statusPanel = new StatusPanel(frame); //TODO

        //Sizers
        wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
        topSizer->Add(configPanel, 0, wxEXPAND);
        topSizer->AddSpacer(20);
        topSizer->Add(videoPanel, 1, wxEXPAND);

        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        rightSizer->Add(settingsPanel, 1, wxEXPAND);
        rightSizer->AddSpacer(10);
        rightSizer->AddStretchSpacer(1);
        rightSizer->Add(downloadPanel, 0, wxEXPAND);

        topSizer->AddSpacer(20);
        topSizer->Add(rightSizer, 0, wxEXPAND);

        //Set up frame
        frame->SetSizer(topSizer);
        frame->SetMinClientSize(wxSize(700, 150));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(JikkenApp);