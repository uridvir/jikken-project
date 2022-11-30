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
        statusPanel = new StatusPanel(topPanel);

        statusPanel->setStatus(StatusPanel::RecReady, true);

        /**
         * The window frame has one child, topPanel. So all the other
         * panels are actually grandchildren of the frame.
         * Having the top panel in between is more proper and it means
         * that background colors all match.
        */

        //Panel sizers
        wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        horizontalSizer->Add(configPanel, 1, wxEXPAND | wxBOTTOM);
        horizontalSizer->AddSpacer(20);
        horizontalSizer->Add(videoPanel, 0, wxEXPAND | wxSHAPED | wxBOTTOM);

        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        rightSizer->Add(settingsPanel, 1, wxEXPAND | wxTOP);
        rightSizer->AddSpacer(10);
        rightSizer->Add(downloadPanel, 0, wxEXPAND | wxBOTTOM);

        horizontalSizer->AddSpacer(20);
        horizontalSizer->Add(rightSizer, 1, wxEXPAND | wxBOTTOM);

        wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
        verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxTOP);
        verticalSizer->AddSpacer(20);
        verticalSizer->Add(statusPanel, 0, wxEXPAND | wxBOTTOM);

        //Set up top panel
        topPanel->SetSizer(verticalSizer);

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