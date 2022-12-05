// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class JikkenFrame : public wxFrame
{
public:
    enum {
        ID_PROPERTIES = 1,
        ID_BUBBLE = 2
    };

    JikkenFrame();
 
private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};