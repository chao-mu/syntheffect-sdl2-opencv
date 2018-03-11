#include "syntheffect/synth/derivative_synth.h"

#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

#define MIN_BLUR_SIZE 0
#define MAX_BLUR_SIZE 100

namespace syntheffect {
    DerivativeSynth::DerivativeSynth() {
        blurSize = cv::Size(3, 3);
    }

    void DerivativeSynth::update(const cv::Mat& in, cv::Mat& out) {
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

    void DerivativeSynth::setBlurSize(double factor) {
        int width = remapParamOdd(factor, MIN_BLUR_SIZE, MAX_BLUR_SIZE);
        blurSize = cv::Size(width, width);
    }

    std::string DerivativeSynth::stringify() {
        return str(boost::format("DerivativeSynth: blurSize(%1%, %2%)") % blurSize.width % blurSize.height);
    }
};

#undef MIN_BLUR_SIZE
#undef MAX_BLUR_SIZE
