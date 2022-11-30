// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class StatusPanel : public wxPanel
{
public:
    StatusPanel(wxWindow* parent);

    enum JikkenState {
        Off, RecReady, Recording, ReadyToDownload, Downloading
    };

    void setStatus(JikkenState state, bool alsoHasDownload = false);
private:
    wxButton *off, *recReady, *recording, *readyToDownload, *downloading;
};