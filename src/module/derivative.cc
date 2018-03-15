#include "syntheffect/module/derivative.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        Derivative::Derivative() {
            blur_size_ = cv::Size(3, 3);
        }

        void Derivative::setBlurSize(int size, int in_min, int in_max) {
            size = remapParamOdd(size, 1, 200, in_min, in_max);
            blur_size_.width = size;
            blur_size_.height = size;
        }

        void Derivative::update(const cv::Mat& in, cv::Mat& out) {
            if (!active_) {
                in.copyTo(out);
                return;
            }

            cv::Mat color;
            cv::blur(in, color, blur_size_);
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
    };
};
