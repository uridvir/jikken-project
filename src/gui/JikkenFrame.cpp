#include "JikkenFrame.h"
#include "PropertiesFrame.h"
#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

/**
 * Important note for GUI code! Japanese text needs to be given in wide strings
 * (16-bit rather than 8-bit). If you try to give wxWidgets narrow strings with
 * Japanese text, it will not render correctly on Japanese machines!!!
 * Please see https://docs.wxwidgets.org/3.0/overview_unicode.html
*/

JikkenFrame::JikkenFrame()
    : wxFrame(nullptr, wxID_ANY, L"じっけん", wxPoint(50, 50), wxSize(800, 600))
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_PROPERTIES, L"&プロパティ\tCtrl-P",
        L"自動ダウンロード、バブル分析オプション、など．．．");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, L"出る"); //TODO: Add status bar text in Japanese

    wxMenu* menuBubble = new wxMenu();
    menuBubble->Append(ID_BUBBLE, L"ファイルを開く\tCtrl-B",
        L"ファイルと、アルゴリズムがバブル分析をします。");

    wxMenu *menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT, L"じっけんプログラム事",
        L"もっとじっけんプログラムの方法を教えられます。");
 
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, L"&ファイル");
    menuBar->Append(menuBubble, L"&バブル分析");
    menuBar->Append(menuHelp, L"&ヘルプ");
 
    SetMenuBar(menuBar);
 
    CreateStatusBar();
    SetStatusText(L"じっけんプログラムには．．．ただ今！！");
 
    Bind(wxEVT_MENU, &JikkenFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &JikkenFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &JikkenFrame::OnProperties, this, ID_PROPERTIES);
    Bind(wxEVT_MENU, &JikkenFrame::OnBubble, this, ID_BUBBLE);
}
 
void JikkenFrame::OnExit(wxCommandEvent& event)
{
    bool alsoHasDownload;
    StatusSetter::JikkenState state = jikkenGlobals.getStatus(alsoHasDownload);

    const std::string harshWarning = "ただ今、プログラムから出ることが大変！"; //Right now, exiting the program is perilous!
    const std::string downloadAvailable = "もうダウンロード出来るビデオがありますよ！"; //Video that can be downloaded already exists!
    const std::string currentDownload = "今こそ、プログラムがビデオをダウンロードしています！"; //Now, the program is downloading video!
    const std::string youWillLose = "プログラムから出たら全部を落とします。"; //If you exit, it will all be lost.
    const std::string exitQuestion = "プログラムから出ますか？"; //Exit the program?

    std::string message;
    if (alsoHasDownload || state == StatusSetter::Downloading) message += harshWarning;
    if (alsoHasDownload) message += downloadAvailable;
    if (state == StatusSetter::Downloading) message += currentDownload;
    if (alsoHasDownload || state == StatusSetter::Downloading) message += youWillLose;
    message += exitQuestion;

    wxMessageDialog exitPrompt(this, //Parent window
        wxString::FromUTF8(message),
        L"プログラム出事", //Caption ("Program quit")
        wxYES_NO //Style
    );
    exitPrompt.SetYesNoLabels("&はい", "&いいえ");
    int quit = exitPrompt.ShowModal();

    if (quit == wxID_YES) Destroy();
}
 
void JikkenFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(L"じっけんちゃんは、じっけんちゃんが何わかんないよ～",
                 L"じっけんプログラム方法", wxOK | wxICON_INFORMATION);
}

void JikkenFrame::OnProperties(wxCommandEvent& event){
    PropertiesFrame* propertiesFrame = new PropertiesFrame(this);
    propertiesFrame->Show();
}

void JikkenFrame::OnBubble(wxCommandEvent& event){

}