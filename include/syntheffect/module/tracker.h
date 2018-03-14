#ifndef SYNTHEFFECT_MODULE_TRACKER_H
#define SYNTHEFFECT_MODULE_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/tracking.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class Tracker : public Module {
            public:
                Tracker();
                void update(const cv::Mat& in, cv::Mat& out);
            private:
                cv::Ptr<cv::Tracker> tracker;
        };
    }
}
#endif
