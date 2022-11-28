#include "JikkenFrame.h"

JikkenFrame::JikkenFrame()
    : wxFrame(nullptr, wxID_ANY, "Jikken じっけん", wxPoint(50, 50), wxSize(800, 600))
{
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("じっけんちゃんから．．．こんにちは！！");
 
    Bind(wxEVT_MENU, &JikkenFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &JikkenFrame::OnExit, this, wxID_EXIT);
}
 
void JikkenFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void JikkenFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("じっけんちゃんは、じっけんちゃんが何わかんないよ～",
                 "じっけんプログラム方法", wxOK | wxICON_INFORMATION);
}