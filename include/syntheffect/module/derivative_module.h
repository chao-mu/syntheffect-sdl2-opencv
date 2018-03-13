#ifndef SYNTHEFFECT_MODULE_DERIVATIVE_MODULE
#define SYNTHEFFECT_MODULE_DERIVATIVE_MODULE

#include <opencv2/core/mat.hpp>

#include "syntheffect/module.h"

namespace syntheffect {
    class DerivativeModule : public Module {
        public:
           DerivativeModule();
           void update(const cv::Mat& in, cv::Mat& out);
           void setBlurSize(double param);
           std::string stringify();

        private:
           cv::Size blurSize;
    };
};
#endif
