#ifndef SYNTHEFFECT_MODULE_COLOR_TWEAK_MODULE
#define SYNTHEFFECT_MODULE_COLOR_TWEAK_MODULE
#include <opencv2/core/mat.hpp>

#include "syntheffect/module.h"

namespace syntheffect {
    class ColorTweakModule : public Module {
        public:
           ColorTweakModule();
           void update(const cv::Mat& in, cv::Mat& out);
    };
}
#endif
