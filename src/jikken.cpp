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
    wxPanel* topPanel;
    ConfigPanel* configPanel;
    VideoPanel* videoPanel;
    SettingsPanel* settingsPanel;
    DownloadPanel* downloadPanel;
    StatusPanel* statusPanel;

public:
    bool OnInit(){
        //Make elements
        frame = new JikkenFrame();
        topPanel = new wxPanel(frame);
        configPanel = new ConfigPanel(topPanel);
        videoPanel = new VideoPanel(topPanel);
        settingsPanel = new SettingsPanel(topPanel);
        downloadPanel = new DownloadPanel(topPanel);
        statusPanel = new StatusPanel(topPanel); //TODO

        /**
         * The window frame has one child, topPanel. So all the other
         * panels are actually grandchildren of the frame.
         * Having the top panel in between is more proper and it means
         * that background colors all match.
        */

        //Panel sizers
        wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
        panelSizer->Add(configPanel, 1, wxEXPAND);
        panelSizer->AddSpacer(20);
        panelSizer->Add(videoPanel, 0, wxEXPAND | wxSHAPED);

        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        rightSizer->Add(settingsPanel, 1, wxEXPAND | wxTOP);
        rightSizer->AddSpacer(10);
        rightSizer->Add(downloadPanel, 0, wxEXPAND | wxBOTTOM);

        panelSizer->AddSpacer(20);
        panelSizer->Add(rightSizer, 1, wxEXPAND);

        //Set up top panel
        topPanel->SetSizer(panelSizer);

        //Frame sizer
        wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
        frameSizer->Add(topPanel, 1, wxEXPAND);

        //Set up frame
        frame->SetSizerAndFit(frameSizer);
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(JikkenApp);