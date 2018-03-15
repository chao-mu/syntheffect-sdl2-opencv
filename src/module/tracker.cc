#include "syntheffect/module/tracker.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

namespace syntheffect {
    namespace module {
        Tracker::Tracker() {
        }

        void Tracker::update(const cv::Mat& in, cv::Mat& out) {
            in.copyTo(out);

            if (!active_) {
                return;
            }

            // Define initial bounding box
            cv::Rect2d roi = cv::selectROI(out, false);
            if (roi.width==0 || roi.height==0) {
                return;
            }

            cv::rectangle(out, roi, cv::Scalar(255, 0, 0), 2, 1);
        }
    };
};
