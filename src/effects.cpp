#include <opencv2/imgproc.hpp>

#include "effects.h"

void duelColorMap(const cv::Mat& frame, cv::Mat& out, int mapBG, int mapFG)
{
    cv::Mat blurred, gray, bw, combined, colored, background;

    cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);

    // Separate foreground
    cv::blur(gray, blurred, cv::Size(3, 3), cv::Point(-1,-1)); 
    cv::threshold(blurred, bw, 100, 255, cv::THRESH_BINARY);
    cv::applyColorMap(gray, colored, mapFG);
    cv::cvtColor(bw, bw, cv::COLOR_GRAY2RGB);
    cv::bitwise_and(colored, bw, out);

    // Apply background
    cv::bitwise_not(bw, background);
    cv::cvtColor(gray, gray, cv::COLOR_GRAY2RGB);
    cv::bitwise_and(gray, background, background);
    cv::applyColorMap(background, background, mapBG);
    cv::bitwise_or(background, out, out);
}
