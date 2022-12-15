#include "PropertiesFrame.h"
#include "JikkenGlobals.h"

extern JikkenGlobals jikkenGlobals;

PropertiesFrame::PropertiesFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, L"プロパティ", wxPoint(50, 50), wxSize(300, 400)) {
    wxPanel* panel = new wxPanel(this);

    //Make elements
    std::vector<std::string> visibleProps; //Props that are not of NODISPLAY type
    const auto& props = jikkenGlobals.propertiesList();
    std::copy_if(props.begin(), props.end(), std::back_inserter(visibleProps), [](auto prop) {
        return jikkenGlobals.propertyType(prop) != "NODISPLAY";
    });

    wxGridSizer* grid = new wxGridSizer(visibleProps.size(), 2, wxSize(10, 10));

    std::map<std::string, std::function<std::string()>> propGetter;

    for (std::string prop : visibleProps){
        std::string type = jikkenGlobals.propertyType(prop);

        //Japanese text makes locale/formatting harder
        wxString localeSafeLabel = wxString::FromUTF8(jikkenGlobals.propertyDisplayName(prop));

        grid->Add(new wxStaticText(panel, wxID_ANY, localeSafeLabel), 1, wxEXPAND);
        
        if (type == "BOOL"){
            wxChoice* choice = new wxChoice(panel, wxID_ANY);
            choice->AppendString("ON");
            choice->AppendString("OFF");
            choice->SetStringSelection(jikkenGlobals.getProperty(prop));
            grid->Add(choice, 1, wxEXPAND);
            propGetter.insert({prop, [choice]() { return std::string(choice->GetStringSelection()); }});
        }
        if (type == "TEXT"){
            wxTextCtrl* text = new wxTextCtrl(panel, wxID_ANY);
            text->SetValue(jikkenGlobals.getProperty(prop));
            grid->Add(text, 1, wxEXPAND);
            propGetter.insert({prop, [text]() { return std::string(text->GetValue()); }});
        }
        if (type.find("INT") == 0) {
            int colonIndex = type.find(":");
            int lowerBound = std::atoi(type.substr(3, colonIndex - 3).c_str());
            int upperBound = std::atoi(type.substr(colonIndex + 1).c_str());

            wxChoice* choice = new wxChoice(panel, wxID_ANY);
            for (int i = lowerBound; i <= upperBound; i++) choice->AppendString(std::to_string(i));
            choice->SetStringSelection(jikkenGlobals.getProperty(prop));
            grid->Add(choice, 1, wxEXPAND);
            propGetter.insert({prop, [choice]() { return std::string(choice->GetStringSelection()); }});
        }
    }

    wxButton* okButton = new wxButton(panel, wxID_ANY, "&OK");
    wxButton* cancelButton = new wxButton(panel, wxID_ANY, L"&キャンセル");

    //Panel sizer
    wxBoxSizer* vertical = new wxBoxSizer(wxVERTICAL);
    vertical->Add(grid, 1, wxEXPAND);
    vertical->AddSpacer(10);

    wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
    buttons->Add(okButton, 1, wxEXPAND);
    buttons->AddSpacer(10);
    buttons->Add(cancelButton, 1, wxEXPAND);

    vertical->Add(buttons, 0, wxEXPAND);

    //Bind callbacks
    okButton->Bind(wxEVT_BUTTON, [this, visibleProps, propGetter](auto event) {
        for (auto prop : visibleProps)
            jikkenGlobals.setProperty(prop, propGetter.at(prop)());
        Close();
    });
    cancelButton->Bind(wxEVT_BUTTON, [this](auto event) { Close(); });

    //Setup panel
    panel->SetSizer(vertical);
    panel->Layout();

    //Frame sizer
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(panel, 1, wxEXPAND);

    //Setup frame
    SetSizer(frameSizer);
    CenterOnParent();
}