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

#include <wx/gbsizer.h>
 
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
        wxGridBagSizer* gbSizer = new wxGridBagSizer(20, 20);
        gbSizer->Add(configPanel, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND);
        gbSizer->Add(videoPanel, wxGBPosition(0, 1), wxGBSpan(1, 1), wxSHAPED | wxALIGN_CENTER_HORIZONTAL);

        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        rightSizer->Add(settingsPanel, 1, wxEXPAND | wxTOP);
        rightSizer->AddSpacer(10);
        rightSizer->Add(downloadPanel, 0, wxEXPAND | wxBOTTOM);

        gbSizer->Add(rightSizer, wxGBPosition(0, 2), wxGBSpan(1, 1), wxEXPAND);
        gbSizer->Add(statusPanel, wxGBPosition(1, 0), wxGBSpan(1, 3), wxEXPAND);
        
        gbSizer->AddGrowableCol(1, 1);
        gbSizer->AddGrowableRow(0, 1);

        //Set up top panel
        topPanel->SetSizer(gbSizer);

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