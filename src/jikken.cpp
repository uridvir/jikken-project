// wxWidgets "Hello World" Program
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
class JikkenApp : public wxApp
{
public:
    virtual bool OnInit();
};
 
class JikkenFrame : public wxFrame
{
public:
    JikkenFrame();
 
private:
    // void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
 
// enum
// {
//     ID_Hello = 1
// };

wxIMPLEMENT_APP(JikkenApp);
 
bool JikkenApp::OnInit()
{
    JikkenFrame *frame = new JikkenFrame();
    frame->Show(true);
    return true;
}
 
JikkenFrame::JikkenFrame()
    : wxFrame(nullptr, wxID_ANY, "Jikken じっけん")
{
    // wxMenu *menuFile = new wxMenu;
    // menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
    //                  "Help string shown in status bar for this menu item");
    // menuFile->AppendSeparator();
    // menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    // menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    // CreateStatusBar();
    // SetStatusText("じっけんちゃんから．．．こんにちは！！");
 
    // Bind(wxEVT_MENU, &JikkenFrame::OnHello, this, ID_Hello);
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
 
// void JikkenFrame::OnHello(wxCommandEvent& event)
// {
//     wxLogMessage("じっけんちゃんから．．．こんにちは！！");
// }
