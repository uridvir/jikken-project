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
#include "CameraController.h"
#include "JikkenGlobals.h"

#include <wx/gbsizer.h>

JikkenGlobals jikkenGlobals;
 
class JikkenApp : public wxApp, public MainManager
{
    JikkenFrame* frame;
    wxPanel* topPanel;
    ConfigPanel* configPanel;
    VideoPanel* videoPanel;
    SettingsPanel* settingsPanel;
    DownloadPanel* downloadPanel;
    StatusPanel* statusPanel;
    CameraController camCtrl;

public:
    bool OnInit(){
        //Make elements
        frame = new JikkenFrame();
        topPanel = new wxPanel(frame);
        configPanel = new ConfigPanel(topPanel, &camCtrl);
        videoPanel = new VideoPanel(topPanel);
        settingsPanel = new SettingsPanel(topPanel, &camCtrl);
        downloadPanel = new DownloadPanel(topPanel, &camCtrl);
        statusPanel = new StatusPanel(topPanel);

        //Globals
        jikkenGlobals = JikkenGlobals(this, statusPanel, configPanel);

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

        //Camera set up (act as though user has just clicked OK to try first batch of settings)
        camCtrl.assignMonitor(videoPanel);
        configPanel->set(jikkenGlobals.getProperty("SERIALPORT"), jikkenGlobals.getProperty("CAMERAID"));

        return true;
    }

    void update(Message msg) override {
        switch(msg){
            case CameraSetupComplete:
                settingsPanel->updateFields();
                downloadPanel->enable(true);
                break;
            case CameraOnlyMode:
                settingsPanel->updateFields(false); //Disable controls
                downloadPanel->enable(false);
                break;
            case NormalQuit:
                frame->OnExit(wxCommandEvent());
                break;
            case UnlockAllCameraControls:
                configPanel->enableEdit(true);
                settingsPanel->updateFields();
                downloadPanel->enable(true);
                break;
            case LockAllCameraControls:
                configPanel->enableEdit(false);
                settingsPanel->updateFields(false);
                downloadPanel->enable(false);
                break;
        }
    }
};

wxIMPLEMENT_APP(JikkenApp);