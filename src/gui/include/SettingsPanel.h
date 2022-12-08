// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "CameraController.h"

class SettingsPanel : public wxPanel
{
    wxChoice* framerate;
    wxStaticText* framerateLabel;
    wxTextCtrl* resolution;
    wxStaticText* resolutionLabel;
    wxChoice* shutterspeed;
    wxStaticText* shutterspeedLabel;
    wxChoice* triggerMode;
    wxStaticText* triggerLabel;
    CameraController* camCtrl;
public:
    SettingsPanel(wxWindow* parent, CameraController* camCtrl);
    void OnFramerateChange(wxCommandEvent& event);
    void OnShutterspeedChange(wxCommandEvent& event);
    void OnTriggerModeChange(wxCommandEvent& event);
    void updateFields();
};