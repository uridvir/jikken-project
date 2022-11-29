#include "VideoPanel.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <wx/sstream.h>
#include <fstream>

VideoPanel::VideoPanel(wxWindow* parent) : wxImagePanel(parent){
    wxInitAllImageHandlers();
    // StretchImage();
    CenterImage();

    //Initial image
    cv::Mat blueScreen(cv::Size(720, 480), CV_8UC3);
    blueScreen.setTo(cv::Scalar(255, 0, 0)); //OpenCV color order is Blue Green Red

    // Layout();

    //Simply use the normal image set routine
    setImageMat(blueScreen);
}

void VideoPanel::setImageMat(const cv::Mat& mat){
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    original = wxImage(mat.cols, mat.rows, rgb.data);

    SetImage(original.Copy());
    // createScaled();
    // m_image = original.Copy();
    
    // std::vector<uchar> buffer(250 * 1024); //Reserve 250KB for PNG in memory
    // cv::imencode(".png", mat, buffer);

    // std::ofstream out("blue.png", std::ios::binary);
    // out << std::string(buffer.begin(), buffer.end());
    // out.close();

    // // wxStringInputStream stream(std::string(buffer.begin(), buffer.end()));
    // // LoadFile(stream, wxBITMAP_TYPE_PNG);

    // LoadFile("blue.png", wxBITMAP_TYPE_PNG);
}

// void VideoPanel::Refresh(bool eraseBackground, const wxRect* rect) {
//     //wxImagePanel::Refresh(eraseBackground, rect); //Call parent

//     if (original.IsOk())
//         createScaled(); //Scale first
// }

// void VideoPanel::createScaled(){
//     int panelWidthI, panelHeightI;
//     GetClientSize(&panelWidthI, &panelHeightI);
//     double panelWidth = panelWidthI, panelHeight = panelHeightI;
//     double originalWidth = (double) original.GetWidth();
//     double originalHeight = (double) original.GetHeight();

//     m_image = original.Copy();
//     if (originalWidth / originalHeight < panelWidth / panelHeight)
//         m_image.Rescale(originalWidth * panelHeight / originalHeight, panelHeight);
//     else
//         m_image.Rescale(panelWidth, originalHeight * panelWidth / originalWidth);
// }