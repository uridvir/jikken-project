#include "SettingsPanel.h"

#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

SettingsPanel::SettingsPanel(wxWindow* parent, CameraController* camCtrl) : wxPanel(parent) {
    this->camCtrl = camCtrl;

    // Make elements
    framerate = new wxChoice(this, wxID_ANY);
    framerate->Append({"1000", "2000", "3000"});
    framerate->SetSelection(0);
    framerateLabel = new wxStaticText(this, wxID_ANY, L"フレーム/秒");

    resolution = new wxTextCtrl(this, wxID_ANY,
                                "256 x 240",                       // Text
                                wxDefaultPosition, wxDefaultSize,  // Use defaults
                                wxTE_READONLY                      // Style
    );
    resolutionLabel = new wxStaticText(this, wxID_ANY, L"解像度");

    shutterspeed = new wxChoice(this, wxID_ANY);
    shutterspeed->Append({"1/1000", "1/2000", "1/3000"});
    shutterspeed->SetSelection(0);
    shutterspeedLabel = new wxStaticText(this, wxID_ANY, L"シャッター速度");

    triggerMode = new wxChoice(this, wxID_ANY);
    triggerMode->Append({"START", "CENTER", "END"});
    triggerMode->SetSelection(0);
    triggerLabel = new wxStaticText(this, wxID_ANY, L"トリガー類");

    // Sizer
    wxGridSizer* sizer = new wxGridSizer(4, 2, wxSize(30, 10));
    sizer->Add(framerate, 1, wxEXPAND);
    sizer->Add(framerateLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    sizer->Add(resolution, 1, wxEXPAND);
    sizer->Add(resolutionLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    sizer->Add(shutterspeed, 1, wxEXPAND);
    sizer->Add(shutterspeedLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    sizer->Add(triggerMode, 1, wxEXPAND);
    sizer->Add(triggerLabel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    // Setup callbacks
    framerate->Bind(wxEVT_CHOICE, &SettingsPanel::OnFramerateChange, this);
    shutterspeed->Bind(wxEVT_CHOICE, &SettingsPanel::OnShutterspeedChange, this);
    triggerMode->Bind(wxEVT_CHOICE, &SettingsPanel::OnTriggerModeChange, this);

    // Disable until camera is set up
    framerate->Enable(false);
    shutterspeed->Enable(false);
    triggerMode->Enable(false);

    // Setup panel
    SetSizer(sizer);
    Layout();
}

void SettingsPanel::OnFramerateChange(wxCommandEvent& event) {
    jikkenGlobals.update(MainManager::Message::LockAllCameraControls);
    std::thread([this]() {
        std::string framerateValue = framerate->GetStringSelection();
        camCtrl->setCameraProperty("FRAMERATE", framerateValue);
        jikkenGlobals.update(MainManager::Message::UnlockAllCameraControls);
    }).detach();
}

void SettingsPanel::OnShutterspeedChange(wxCommandEvent& event) {
    jikkenGlobals.update(MainManager::Message::LockAllCameraControls);
    std::thread([this]() {
        std::string shutterspeedValue = shutterspeed->GetStringSelection();
        camCtrl->setCameraProperty("SHUTTERSPEED", shutterspeedValue);
        jikkenGlobals.update(MainManager::Message::UnlockAllCameraControls);
    }).detach();
}

void SettingsPanel::OnTriggerModeChange(wxCommandEvent& event) {
    jikkenGlobals.update(MainManager::Message::LockAllCameraControls);
    std::thread([this]() {
        std::string triggerModeValue = triggerMode->GetStringSelection();
        camCtrl->setCameraProperty("TRIGGERMODE", triggerModeValue);
        jikkenGlobals.update(MainManager::Message::UnlockAllCameraControls);
    }).detach();
}

void SettingsPanel::updateFields(bool enable) {
    framerate->Enable(enable);
    shutterspeed->Enable(enable);
    triggerMode->Enable(enable);

    if (!enable) return;

    framerate->SetStringSelection(camCtrl->getCameraProperty("FRAMERATE"));
    resolution->SetValue(camCtrl->getCameraProperty("RESOLUTION"));
    shutterspeed->SetStringSelection(camCtrl->getCameraProperty("SHUTTERSPEED"));
    triggerMode->SetStringSelection(camCtrl->getCameraProperty("TRIGGERMODE"));
}