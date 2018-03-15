#ifndef SYNTHEFFECT_MODULE_MERGER_H
#define SYNTHEFFECT_MODULE_MERGER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "syntheffect/module/module.h"
#include <cstdint>

namespace syntheffect {
    namespace module {
        class Merger : public Module {
            public:
                Merger(double weight_right=0);
                void update(const cv::Mat& left, const cv::Mat& right, cv::Mat& out);
                void setWeight(double x, double min_in, double max_in);
                void fadeWeight(bool up);
                std::string stringify();

            private:
                double weight_;
        };
    }
}
#endif
