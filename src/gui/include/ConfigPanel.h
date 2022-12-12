// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "CameraController.h"
#include "Logger.h"

class ConfigPanel : public wxPanel, public Logger
{
    wxTextCtrl* serialPort;
    wxStaticText* serialLabel;
    wxChoice* cameraID;
    wxStaticText* cameraLabel;
    wxTextCtrl* logBox;
    wxButton* okButton;
    wxButton* editButton;
    CameraController* camCtrl;
public:
    ConfigPanel(wxWindow* parent, CameraController* camCtrl);
    void OnOK(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void log(std::string text) override;
    void set(std::string serial, std::string id); //Differs from class diagram
};