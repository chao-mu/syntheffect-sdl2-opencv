#include "syntheffect/module/color_tweak_module.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    ColorTweakModule::ColorTweakModule() {
    }

    void ColorTweakModule::update(const cv::Mat& in, cv::Mat& out) {
        if (!active_) {
            in.copyTo(out);
            return;
        }
        //cv::Size blur_size(5, 5);
        //cv::blur(in, out, blur_size);

        cv::cvtColor(in, out, cv::COLOR_BGR2HSV);

        uint32_t offset = SDL_GetTicks() / 10;
        for (int row = 0; row < out.rows; row++) {
            for (int col = 0; col < out.cols; col++) {
                out.at<uchar>(row, col, 0) = (out.at<uchar>(row, col, 0) + offset ) % 256;
                out.at<uchar>(row, col, 2) = (out.at<uchar>(row, col, 2) + (offset / 2)) % 256;
            }
        }

        cv::cvtColor(out, out, cv::COLOR_HSV2BGR);
    }
};
