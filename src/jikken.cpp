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
        wxFlexGridSizer* horizontalSizer = new wxFlexGridSizer(1, 3, wxSize(20, 0));
        horizontalSizer->Add(configPanel, 1, wxEXPAND);
        horizontalSizer->Add(videoPanel, 1, wxEXPAND | wxSHAPED | wxALIGN_CENTER_HORIZONTAL);

        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        rightSizer->Add(settingsPanel, 1, wxEXPAND | wxTOP);
        rightSizer->AddSpacer(10);
        rightSizer->Add(downloadPanel, 0, wxEXPAND | wxBOTTOM);

        horizontalSizer->Add(rightSizer, 1, wxEXPAND);

        horizontalSizer->AddGrowableCol(1, 1);
        horizontalSizer->AddGrowableRow(0, 1);
        // horizontalSizer->AddGrowableCol(0, 1);
        // horizontalSizer->AddGrowableCol(2, 1);

        // wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
        // verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxTOP);
        // verticalSizer->AddSpacer(20);
        // verticalSizer->Add(statusPanel, 0, wxEXPAND | wxBOTTOM);

        //Set up top panel
        // topPanel->SetSizer(verticalSizer);
        topPanel->SetSizer(horizontalSizer);

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