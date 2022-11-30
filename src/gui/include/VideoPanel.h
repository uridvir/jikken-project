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
};