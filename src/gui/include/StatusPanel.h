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
private:
    wxButton *off, *readyToRecord, *recording, *readyToDownload, *downloading;
};