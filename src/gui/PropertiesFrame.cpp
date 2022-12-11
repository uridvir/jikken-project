#include "PropertiesFrame.h"

PropertiesFrame::PropertiesFrame(){
    wxPanel* panel = new wxPanel(this);

    wxGridSizer* grid = new wxGridSizer(propList.size() + 1, 2, wxSize(10, 10));

    std::map<std::string, std::function<std::string()>> propGetter;

    for (auto line : propList){
        std::string prop = line[0];
        std::string type = line[1];

        grid->Add(new wxStaticText(this, wxID_ANY, propDisplayName[prop]), 1, wxEXPAND);
        
        if (type == "BOOL"){
            wxChoice* choice = new wxChoice(this, wxID_ANY);
            // choice->Append({"ON", "OFF"});
            // choice->SetStringSelection(jikkenGlobals.jikkenPropertiesHolder.getProperty(prop));
            grid->Add(choice, 1, wxEXPAND);
            propGetter.insert({prop, [choice]() -> std::string { return choice->GetStringSelection(); }});
        }
        if (type == "TEXT"){
            wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY);
            // text->SetValue(jikkenGlobals.jikkenPropertiesHolder.getProperty(prop));
            grid->Add(text, 1, wxEXPAND);
            propGetter.insert({prop, [text]() -> std::string { return text->GetValue(); }});
        }
    }

    wxButton* okButton = new wxButton(this, wxID_ANY, "&OK");
    wxButton* cancelButton = new wxButton(this, wxID_ANY, "&キャンセル");

    wxBoxSizer* vertical = new wxBoxSizer(wxVERTICAL);
    vertical->Add(grid, 1, wxEXPAND);
    vertical->AddSpacer(10);

    wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
    buttons->Add(okButton, 0);
    buttons->AddSpacer(10);
    buttons->Add(cancelButton, 0);

    vertical->Add(buttons, 0);

    okButton->Bind(wxEVT_BUTTON, [this, propGetter]() {
        for (auto line : propList) {
            std::string prop = line[0];
            // jikkenGlobals.jikkenPropertiesHolder.setProperty(prop, propGetter.at(prop)());
        }
        Close();
    });
    cancelButton->Bind(wxEVT_BUTTON, [this]() { Close(); });

    SetSizer(vertical);
    Layout();
}