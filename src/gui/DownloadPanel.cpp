#include "DownloadPanel.h"

DownloadPanel::DownloadPanel(wxWindow* parent) : wxPanel(parent){
    //Make elements
    wxButton* recordButton = new wxButton(this, wxID_ANY, "レコード");
    wxButton* downloadButton = new wxButton(this, wxID_ANY, "ダウンロード");

    //Sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(recordButton, wxLEFT);
    sizer->Add(downloadButton, wxRIGHT);

    //Setup panel
    SetSizer(sizer);
    Layout();
}