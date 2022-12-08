#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent, CameraController* camCtrl) : wxPanel(parent){
    this->camCtrl = camCtrl;

    //Make elements
    serialPort = new wxTextCtrl(this, wxID_ANY, "COM3");
    serialLabel = new wxStaticText(this, wxID_ANY, "シリアルポート");
  
    cameraID = new wxChoice(this, wxID_ANY);
    for (int i = 0; i <= 10; i++)
        cameraID->Append(std::to_string(i));
    cameraID->SetSelection(0);
    cameraLabel = new wxStaticText(this, wxID_ANY, "カメラID");

    logBox = new wxTextCtrl(this, wxID_ANY, 
        "", wxDefaultPosition, wxDefaultSize, //Use defaults
        wxTE_MULTILINE | wxTE_READONLY //Style
    );

    okButton = new wxButton(this, wxID_ANY, "OK");
    editButton = new wxButton(this, wxID_ANY, "エディット");
    editButton->Enable(false);

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

    //Setup callbacks
    okButton->Bind(wxEVT_BUTTON, &ConfigPanel::OnOK, this);
    editButton->Bind(wxEVT_BUTTON, &ConfigPanel::OnEdit, this);

    //Setup panel
    SetSizer(topSizer);
    Layout();
}

void ConfigPanel::OnOK(wxCommandEvent& event){
    //Fetch config settings
    std::string serial = this->serialPort->GetValue();
    int id = this->cameraID->GetSelection();

    //Lock
    this->serialPort->Enable(false);
    this->cameraID->Enable(false);
    okButton->Enable(false);
    editButton->Enable(true);

    //Try connect to camera
    bool success = camCtrl->config(serial, id);
    if (success) return;

    //If failure, prompt user
    wxMessageDialog askNewSettings(this, //Parent window
        /**
         * The camera could not be set up. The serial port and camera ID don't work. Try new settings?
        */ 
        "カメラには、セットアップを出来ませんでした。シリアルポートとカメラIDには、行けませんから。新しいオプションをしてみますか？",
        // ^ message
        "カメラセットアップ", //Caption ("Camera Setup")
        wxYES_NO //Style
    );
    askNewSettings.SetYesNoLabels("&はい", "&いいえ");

    wxMessageDialog askNoCamera(this,
        /**
         * Start the program without the camera? You can also quit the program now.
        */
        "じっけんプログラムがカメラなし始まって宜しいですか？良かったら、今プログラムから出られます。", //Message
        "カメラセットアップ", //Caption ("Camera Setup")
        wxYES_NO | wxCANCEL //Style
    );
    askNoCamera.SetYesNoCancelLabels("&はい", "&出る", "&前へ");

    while (true){
        int newSettings = askNewSettings.ShowModal();
        if (newSettings == wxID_YES){
            OnEdit(wxCommandEvent());
            break;
        }
        //TODO: askNoCamera logic
    }
}

void ConfigPanel::OnEdit(wxCommandEvent& event){
    //Unlock
    this->serialPort->Enable(true);
    this->cameraID->Enable(true);
    okButton->Enable(true);
    editButton->Enable(false);
}

void ConfigPanel::log(std::string text){
    std::string logText = this->logBox->GetValue();
    logText += "\n" + text;
    this->logBox->SetValue(logText);
}

void ConfigPanel::set(std::string serial, int id){
    this->serialPort->ChangeValue(serial);
    this->cameraID->SetSelection(id);
    OnOK(wxCommandEvent()); //Pretend OK button was pressed, and give dummy event
}