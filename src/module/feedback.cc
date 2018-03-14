#include "syntheffect/module/feedback.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        Feedback::Feedback() {
            weight_ = 0.9;
        }

        void Feedback::update(const cv::Mat& in, cv::Mat& out) {
            if (!active_ || last_frame_.empty()) {
                in.copyTo(out);
                in.copyTo(last_frame_);
                return;
            }

            cv::addWeighted(in, (1 - weight_), last_frame_, weight_, 0, out);
            out.copyTo(last_frame_);
        }

        void Feedback::setWeight(double v, double min, double max) {
            weight_ = remapParam(v, 0, 1, min, max);
        }
    };
};
