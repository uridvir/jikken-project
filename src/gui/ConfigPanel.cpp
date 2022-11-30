#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent) : wxPanel(parent){
    //Make elements
    wxTextCtrl* serialPort = new wxTextCtrl(this, wxID_ANY, "COM3");
    wxStaticText* serialLabel = new wxStaticText(this, wxID_ANY, "シリアルポート");
  
    wxChoice* cameraID = new wxChoice(this, wxID_ANY);
    for (int i = 0; i <= 10; i++)
        cameraID->Append(std::to_string(i));
    cameraID->SetSelection(0);
    wxStaticText* cameraLabel = new wxStaticText(this, wxID_ANY, "カメラID");

    wxTextCtrl* logBox = new wxTextCtrl(this, wxID_ANY, 
        "", wxDefaultPosition, wxDefaultSize, //Use defaults
        wxTE_MULTILINE | wxTE_READONLY //Style
    );

    wxButton* okButton = new wxButton(this, wxID_ANY, "OK");
    wxButton* editButton = new wxButton(this, wxID_ANY, "エディット");

    //Sizer
    wxGridSizer* gridSizer = new wxGridSizer(2, 2, wxSize(30, 10));
    gridSizer->Add(serialPort, 1, wxEXPAND);
    gridSizer->Add(serialLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(cameraID, 1, wxEXPAND);
    gridSizer->Add(cameraLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, wxLEFT);
    buttonSizer->Add(editButton, wxRIGHT);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(gridSizer, 0, wxEXPAND);
    topSizer->AddSpacer(10);
    topSizer->Add(logBox, 1, wxEXPAND);
    topSizer->AddSpacer(10);
    topSizer->Add(buttonSizer, 0, wxEXPAND);

    //Setup panel
    SetSizer(topSizer);
    Layout();
}