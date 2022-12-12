#include "VideoSubscriber.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "imagepanel.h"
#include <opencv2/core.hpp>

class VideoPanel : public wxImagePanel, public VideoSubscriber
{ 
public:
    VideoPanel(wxWindow* parent);
    void onReceiveMat(const cv::Mat& mat) override;
    void onRemove() override;
private:
    void init();
};