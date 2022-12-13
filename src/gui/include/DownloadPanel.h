// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "CameraController.h"

class DownloadPanel : public wxPanel
{
    wxButton* recordButton;
    wxButton* downloadButton;
    CameraController* camCtrl;
public:
    DownloadPanel(wxWindow* parent, CameraController* camCtrl);
    void OnRecord(wxCommandEvent& event);
    void OnDownload(wxCommandEvent& event);
    void enable(bool choice);
};