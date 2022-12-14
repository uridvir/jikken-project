// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "StatusSetter.h"

class StatusPanel : public wxPanel, public StatusSetter
{
public:
    StatusPanel(wxWindow* parent);

    void setStatus(JikkenState state, bool alsoHasDownload = false);
    JikkenState getStatus(bool& alsoHasDownload);
private:
    wxButton *off, *readyToRecord, *recording, *readyToDownload, *downloading;
    JikkenState state = Off;
    bool alsoHasDownload = false;
};