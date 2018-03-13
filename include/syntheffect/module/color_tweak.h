#ifndef SYNTHEFFECT_MODULE_COLOR_TWEAK_H
#define SYNTHEFFECT_MODULE_COLOR_TWEAK_H

#include <opencv2/core/mat.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class ColorTweak : public Module {
            public:
                ColorTweak();
                void update(const cv::Mat& in, cv::Mat& out);
        };
    };
};
#endif
