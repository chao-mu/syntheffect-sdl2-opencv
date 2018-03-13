#include "syntheffect/module/derivative_module.h"

#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

#define MIN_BLUR_SIZE 0
#define MAX_BLUR_SIZE 100

namespace syntheffect {
    DerivativeModule::DerivativeModule() {
        blurSize = cv::Size(3, 3);
    }

    void DerivativeModule::update(const cv::Mat& in, cv::Mat& out) {
        if (!active_) {
            in.copyTo(out);
            return;
        }

        cv::Mat color;
        GaussianBlur(in, color, blurSize, 0);
        cvtColor(color, out, cv::COLOR_BGR2GRAY);

        int ddepth = CV_16S;

        cv::Mat grad_x, grad_y;

        Scharr(out, grad_x, ddepth, 1, 0);
        convertScaleAbs(grad_x, grad_x);

        Scharr(out, grad_y, ddepth, 0, 1);
        convertScaleAbs(grad_y, grad_y);

        addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);

        cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);

        bitwise_and(color, out, out);
    }

    void DerivativeModule::setBlurSize(double factor) {
        int width = remapParamOdd(factor, MIN_BLUR_SIZE, MAX_BLUR_SIZE);
        blurSize = cv::Size(width, width);
    }

    std::string DerivativeModule::stringify() {
        return str(boost::format("DerivativeModule: blurSize(%1%, %2%)") % blurSize.width % blurSize.height);
    }
};

#undef MIN_BLUR_SIZE
#undef MAX_BLUR_SIZE
