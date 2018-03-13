#ifndef SYNTHEFFECT_MODULE_DERIVATIVE_H
#define SYNTHEFFECT_MODULE_DERIVATIVE_H

#include <opencv2/core/mat.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class Derivative : public Module {
            public:
                Derivative();
                void update(const cv::Mat& in, cv::Mat& out);
                void setBlurSize(double param);
                std::string stringify();

            private:
                cv::Size blurSize;
        };
    };
};
#endif
