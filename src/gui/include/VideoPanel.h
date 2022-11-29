// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "imagepanel.h"
#include <opencv2/core.hpp>

class VideoPanel : public wxImagePanel
{ 
public:
    VideoPanel(wxWindow* parent);
    void setImageMat(const cv::Mat& mat);
    // void Refresh(bool, const wxRect*) override; //Override parent method to implement scaling how I want it
private:
    wxImage original;

    // void createScaled();
};