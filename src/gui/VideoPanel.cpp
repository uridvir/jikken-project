#include "VideoPanel.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <wx/sstream.h>
#include <fstream>

VideoPanel::VideoPanel(wxWindow* parent) : wxImagePanel(parent, wxID_ANY, wxDefaultPosition, wxSize(360, 240)){
    wxInitAllImageHandlers(); //wxWidgets doesn't do this automatically for some reason??
    StretchImage(); //Let image panel library do resizing

    //Initial image
    init();
}

void VideoPanel::init(){
    cv::Mat blueScreen(cv::Size(720, 480), CV_8UC3);
    blueScreen.setTo(cv::Scalar(255, 0, 0)); //OpenCV color order is Blue Green Red

    // cv::Rect crop((720 - 512) / 2, 0, 512, 480);
    // cv::Mat subMat = blueScreen(crop);
    // subMat.setTo(cv::Scalar(0, 0, 255));

    //Simply use the normal image set routine
    onReceiveMat(blueScreen);
}

void VideoPanel::onReceiveMat(const cv::Mat& mat){
    //Convert from OpenCV's BGR to RGB
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    //Get wxImage directly from raw image
    wxImage image = wxImage(mat.cols, mat.rows, rgb.data, true);
    SetImage(image.Copy());
}

void VideoPanel::onRemove() { // When the VideoPanel stops getting video frames
    init();
}