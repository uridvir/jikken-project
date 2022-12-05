#include "JikkenFrame.h"

JikkenFrame::JikkenFrame()
    : wxFrame(nullptr, wxID_ANY, "じっけん", wxPoint(50, 50), wxSize(800, 600))
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_PROPERTIES, "&プロパティ\tCtrl-P",
        "自動ダウンロード、バブル分析オプション、など．．．");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "出る");

    wxMenu* menuBubble = new wxMenu();
    menuBubble->Append(ID_BUBBLE, "ファイルを開く\tCtrl-B",
        "ファイルと、アルゴリズムがバブル分析をします。");

    wxMenu *menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT, "じっけんプログラム事",
        "もっとじっけんプログラムの方法を教えられます。");
 
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&ファイル");
    menuBar->Append(menuBubble, "&バブル分析");
    menuBar->Append(menuHelp, "&ヘルプ");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("じっけんプログラムには．．．ただ今！！");
 
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