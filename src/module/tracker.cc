#include "syntheffect/module/tracker.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

namespace syntheffect {
    namespace module {
        Tracker::Tracker() {
        }

        void Tracker::update(const cv::Mat& in, cv::Mat& out) {
            if (!active_) {
                in.copyTo(out);
                return;
            }
        }
    };
};
