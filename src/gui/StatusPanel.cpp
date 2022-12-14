#include "StatusPanel.h"

StatusPanel::StatusPanel(wxWindow* parent) : wxPanel(parent) {
    // Make elements
    off = new wxButton(this, wxID_ANY, L"切");
    off->Enable(false);
    recReady = new wxButton(this, wxID_ANY, L"レコード可");
    recReady->Enable(false);
    recording = new wxButton(this, wxID_ANY, L"レコード中");
    recording->Enable(false);
    readyToDownload = new wxButton(this, wxID_ANY, L"ダウンロード可");
    readyToDownload->Enable(false);
    downloading = new wxButton(this, wxID_ANY, L"ダウンロード中");
    downloading->Enable(false);

    // Sizer
    wxGridSizer* leftSizer = new wxGridSizer(1, 4, wxSize(10, 0));
    leftSizer->Add(off, 1, wxEXPAND);
    leftSizer->Add(recReady, 1, wxEXPAND);
    leftSizer->Add(recording, 1, wxEXPAND);
    leftSizer->Add(readyToDownload, 1, wxEXPAND);

    wxGridSizer* topSizer = new wxGridSizer(1, 2, wxSize(10, 0));
    topSizer->Add(leftSizer, 1, wxEXPAND);
    topSizer->Add(downloading, 1, wxEXPAND);

    // Setup panel
    SetSizer(topSizer);
    Layout();

    setStatus(Off);
}

void StatusPanel::setStatus(JikkenState state, bool alsoHasDownload) {
    // Clear the boxes
    off->SetBackgroundColour(GetBackgroundColour());
    recReady->SetBackgroundColour(GetBackgroundColour());
    recording->SetBackgroundColour(GetBackgroundColour());
    readyToDownload->SetBackgroundColour(GetBackgroundColour());
    downloading->SetBackgroundColour(GetBackgroundColour());

    //"Also download" option
    if (alsoHasDownload) readyToDownload->SetBackgroundColour(wxColour("yellow"));

    // Fill in the new state
    switch (state) {
        case Off:
            off->SetBackgroundColour(wxColour("green"));
            break;
        case RecReady:
            recReady->SetBackgroundColour(wxColour("green"));
            break;
        case Recording:
            recording->SetBackgroundColour(wxColour("green"));
            break;
        case ReadyToDownload:
            readyToDownload->SetBackgroundColour(wxColour("green"));
            break;
        case Downloading:
            downloading->SetBackgroundColour(wxColour("green"));
            break;
    }

    // Since we just changed elements
    Refresh();
}