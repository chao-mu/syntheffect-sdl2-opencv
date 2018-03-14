#ifndef SYNTHEFFECT_MODULE_FEEDBACK_H
#define SYNTHEFFECT_MODULE_FEEDBACK_H

#include <opencv2/core/mat.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class Feedback : public Module {
            public:
                Feedback();
                void update(const cv::Mat& in, cv::Mat& out);
                void setWeight(double v, double min, double max);

            private:
                cv::Mat last_frame_;
                double weight_;
        };
    }
}
#endif
