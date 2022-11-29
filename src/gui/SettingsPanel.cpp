#include "SettingsPanel.h"

SettingsPanel::SettingsPanel(wxWindow* parent) : wxPanel(parent){
    //Make elements
    wxChoice* framerate = new wxChoice(this, wxID_ANY);
    framerate->Append({"1000", "2000", "3000"});
    framerate->SetSelection(0);
    wxStaticText* framerateLabel = new wxStaticText(this, wxID_ANY, "フレーム/秒");

    wxTextCtrl* resolution = new wxTextCtrl(this, wxID_ANY,
        "256 x 240", //Text
        wxDefaultPosition, wxDefaultSize, //Use defaults
        wxTE_READONLY //Style
    );
    wxStaticText* resolutionLabel = new wxStaticText(this, wxID_ANY, "解像度");

    wxChoice* shutterspeed = new wxChoice(this, wxID_ANY);
    shutterspeed->Append({"1000", "2000", "3000"});
    shutterspeed->SetSelection(0);
    wxStaticText* shutterspeedLabel = new wxStaticText(this, wxID_ANY, "シャッター速さ");

    wxChoice* triggerMode = new wxChoice(this, wxID_ANY);
    triggerMode->Append({"START", "CENTER", "END"});
    triggerMode->SetSelection(0);
    wxStaticText* triggerLabel = new wxStaticText(this, wxID_ANY, "トリガー類");

    //Sizer
    wxGridSizer* sizer = new wxGridSizer(4, 2, wxSize(30, 10));
    sizer->Add(framerate, 1, wxEXPAND);
    sizer->Add(framerateLabel, 1, wxEXPAND);
    sizer->Add(resolution, 1, wxEXPAND);
    sizer->Add(resolutionLabel, 1, wxEXPAND);
    sizer->Add(shutterspeed, 1, wxEXPAND);
    sizer->Add(shutterspeedLabel, 1, wxEXPAND);
    sizer->Add(triggerMode, 1, wxEXPAND);
    sizer->Add(triggerLabel, 1, wxEXPAND);

    //Setup panel
    SetSizer(sizer);
    Layout();
}