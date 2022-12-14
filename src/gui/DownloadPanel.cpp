#include "DownloadPanel.h"

#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

DownloadPanel::DownloadPanel(wxWindow* parent, CameraController* camCtrl) : wxPanel(parent) {
    this->camCtrl = camCtrl;

    // Make elements
    recordButton = new wxButton(this, wxID_ANY, L"レコード");
    downloadButton = new wxButton(this, wxID_ANY, L"ダウンロード");
    recordButton->Enable(false);
    downloadButton->Enable(false);

    // Sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(recordButton, wxLEFT);
    sizer->Add(downloadButton, wxRIGHT);

    // Setup callbacks
    recordButton->Bind(wxEVT_BUTTON, &DownloadPanel::OnRecord, this);
    downloadButton->Bind(wxEVT_BUTTON, &DownloadPanel::OnDownload, this);

    // Setup panel
    SetSizer(sizer);
    Layout();
}

void DownloadPanel::OnRecord(wxCommandEvent& event) {
    std::thread([this]() {
        jikkenGlobals.setStatus(StatusSetter::Recording);

        // Lock
        recordButton->Enable(false);
        downloadButton->Enable(false);
        jikkenGlobals.update(MainManager::Message::LockAllCameraControls);

        camCtrl->record();

        // TODO: Auto-download logic

        // Unlock
        jikkenGlobals.update(MainManager::Message::UnlockAllCameraControls);
        recordButton->Enable(true);
        downloadButton->Enable(true);

        jikkenGlobals.setStatus(StatusSetter::ReadyToRecord, true);
    }).detach();
}

void DownloadPanel::OnDownload(wxCommandEvent& event) {
    std::thread([this]() {
        jikkenGlobals.setStatus(StatusSetter::Downloading);

        // Lock
        recordButton->Enable(false);
        downloadButton->Enable(false);
        jikkenGlobals.update(MainManager::Message::LockAllCameraControls);

        camCtrl->download();

        // TODO: File save logic

        // Unlock
        recordButton->Enable(true);
        downloadButton->Enable(false);
        jikkenGlobals.update(MainManager::Message::UnlockAllCameraControls);

        jikkenGlobals.setStatus(StatusSetter::ReadyToRecord);
    }).detach();
}

void DownloadPanel::enable(bool choice) {
    recordButton->Enable(choice);

    static bool downloadPreviouslyEnabled;
    if (!choice) {
        downloadPreviouslyEnabled = downloadButton->IsEnabled();
        downloadButton->Enable(false);
    } else
        downloadButton->Enable(downloadPreviouslyEnabled);
}